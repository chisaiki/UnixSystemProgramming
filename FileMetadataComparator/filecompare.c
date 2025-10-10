#define _GNU_SOURCE    /* Needed to expose statx() function in glibc */
#include <sys/stat.h>  /* Required according to man page for statx() */
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <locale.h>
#include <pwd.h>
#include <grp.h>
#include <sys/sysmacros.h>
#include <time.h>
#include <libgen.h>
#include <stdbool.h> 
#include <stdlib.h> //to use exit
#include <unistd.h> //to get getopt
#include <string.h>

/*count the number of operations exist -> if number of operation is more than 1 then error*/

bool countNumofOptions(int argc, char *argv[])
{
    int optioncount = 0;

    for (int i = 0; i < argc; i++)
    {
        for(int j = 0; j < argc; j++)
        {
          if(argv[i][0] == '-' && (argv[i][j] == 'a' ||  argv[i][j] == 'b' ||  argv[i][j] == 'c' ||  argv[i][j] == 'm' ||  argv[i][j] == 's' ||  argv[i][j] == 'u'))
          {
            optioncount++;

            if( argv[i][0] == '-' && (argv[i][j]== 'r' || argv[i][j] == 'l'))
            {
                optioncount = optioncount + 0;
            }
          }   
        }
    }

    if(optioncount == 1)
        return true;
    else //if no operation is specified then it will error and exit 
        {
            printf("\nOperations wrong amount");
            return false;
            exit(1);
        }
}

/*Function taken for showstat.c by Stewart Weiss*/
void printTime(const char* label, struct statx_timestamp *time_field)
{
    struct tm *bdtime;                 /* Broken-down time                 */
    char       formatted_time[100];    /* String storing formatted time    */
    char       timezone[32];           /* To store time offset             */
    time_t     time_val;               /* For converted tv_sec field       */

    time_val = time_field->tv_sec;          /* Convert to time_t           */
    bdtime = localtime(&time_val);          /* Convert to broken-down time */
    if (bdtime == NULL)                     /* Check for error             */
        fprintf(stderr, "localtime failed");

    /* Create a string from the broken down time. */
    if ( strftime(formatted_time, sizeof(formatted_time), "%F %T", bdtime) == 0 )
        fprintf(stderr,"strftime failed\n");

    printf("%s%s.%09u", label, formatted_time, time_field->tv_nsec );
    if ( 0 == strftime(timezone, 32, " %z", bdtime) )
        fprintf(stderr, "Error printing time zone\n");
    printf("%s\n", timezone);
}

int timetoSeconds(struct statx_timestamp *time_field)
{
    struct tm *bdtime;                 /* Broken-down time                 */
    time_t     time_val;               /* For converted tv_sec field       */

    time_val = time_field->tv_sec;          /* Convert to time_t           */ //time_t is already in seconds since i set it that way
    bdtime = localtime(&time_val);          /* Convert to broken-down time */
    if (bdtime == NULL)                     /* Check for error             */
        fprintf(stderr, "Localtime failed");

    int seconds = bdtime->tm_sec + bdtime->tm_min*60 + bdtime->tm_hour*3600 + bdtime->tm_yday*86400 + (bdtime->tm_year-70)*31536000 + ((bdtime->tm_year-69)/4)*86400 - ((bdtime->tm_year-1)/100)*86400 + ((bdtime->tm_year+299)/400)*86400;

   // time_val = mktime(bdtime);
    //int seconds = time_val;
    return seconds; 
}
struct file_data
{
    char filename[256]; /*Microsoft Windows has a MAX_PATH limit of ~256 characters ---- better to use PATh_MAX since it can vary*/
    long long int size; //make a more generic name , you can just declare long long 
    struct file_data *next;
};
void addLast(struct file_data **head, char *argv, long long int size_input)
{
    /*create a new node and insert data from parameters*/
    struct file_data *newNode = malloc(sizeof(struct file_data));
    strcpy(newNode->filename, argv);
    newNode->size = size_input;
    newNode->next = NULL;

    /*if head is NULL, then there is no list/its empty*/
    if(*head == NULL)
        *head = newNode;
    else
    {
        struct file_data *lastNode = *head;
        /*last node's next address will be NULL.*/
        while(lastNode->next != NULL)
        {
            lastNode = lastNode->next;
        }
        /*add the newNode at the end of the linked list*/
            lastNode->next = newNode;
    }
}
   
   
void printList(struct file_data *head, char operation)
    {
        struct file_data *temp = head;
        int i = 1;

        //iterate the entire linked list and print the data
        while(temp != NULL)
        {

            switch(operation)
            {
                
                case 'm': 
                case 'a':
                case 'b':
                case 'c':
                {
                    printf("\n#%d.Filename: %s", i, temp->filename);
                    i++;
                    break;
                }


                case 'u':
                {
                    printf("\nFilename: %s", temp->filename);
                    printf("\nDisk usage is: %lld", temp->size);
                    printf("\n----------------------------------"
                    "-----------------------------------------\n");
                    break;
                }

                case 's':
                {
                    printf("\nFilename: %s", temp->filename);
                    printf("\nApparent size is: %lld", temp->size);
                    printf("\n----------------------------------"
                    "-----------------------------------------\n");
                    break;
                }
                    
            }
            temp = temp->next;
        }
    }


void sortLinkedList(struct file_data **head) {
  struct file_data *current = *head, *index = NULL;
  int temp;

  if (head == NULL) 
  return;

  else 
  {
  while (current != NULL) {
    // index points to the node next to current
    index = current->next;

    while (index != NULL) {
    if (current->size < index->size) {
      temp = current->size;
      current->size = index->size;
      index->size = temp;
    }
    index = index->next;
    }
    current = current->next;
  }
  }
}


/*****************************START OF MAIN***********************************/
int main(int argc, char *argv[])
{//main start bracket

    /*ERROR HANDLING*/
    /*Invalid reoccurances of operations on command line = Error*/
    if (countNumofOptions(argc, argv) == false)
    exit(1);

    /* At least one filename must be given. If none, no error. Outputs nothing. */
    if (optind >= argc) 
    exit(0);
    /****************************************************************************/

    struct statx statx_buffer;    /* statx structure filled by statx()        */
    unsigned int mask = STATX_ALL; /* mask to pass to statx()                  */
    int          report_on_link = AT_SYMLINK_NOFOLLOW;  /* Flag indicating whether to report on link*/
    ssize_t      nbytes;          /* Return value of readlink()               */
    char         target[256];
    time_t conversion;
    bool r_option = false; 
    int operations;
    char foundoperation;
    struct file_data *head = NULL;

        /*Option Parsing*/
        while((operations = getopt(argc, argv, "abcmsurl")) != -1)
        {
            switch(operations)
            {
                case 'a':
                {
                    //determine the metadata by which the files are compared by their times of last access
                    foundoperation = 'a';
                    break;
                }

                case 'b': // By their birth times
                {
                    foundoperation = 'b';
                    break;
                }

                case 'c': // By their times of last status change
                {
                    foundoperation = 'c';
                    break;
                }

                case 'm': // By their times of last modification
                {
                    foundoperation = 'm';
                    break;
                }

                case 's': // By their apparent size
                {
                    foundoperation = 's';
                    break;
                }

                case 'u': // By their disk usage
                {
                    foundoperation = 'u';
                    break;
                }

                case 'r':
                {
                    r_option = true;
                    break;
                }

                case 'l':
                {
                    report_on_link = 0;
                    break;
                }

                case '?' :
                fprintf(stderr,"\nFound invalid option %c\n", optopt);
            }
        } argc - optind = size of arguments on command line 

        //Maybe here: if -L is on then follow to target 
        //I think i can make my own linked list since I just need to add elements for each file 

   
        /* For each file argument, call statx() and print its metadata. */
        for (int i = optind; i < argc; i++) 
        {
            if (statx(AT_FDCWD, argv[i], report_on_link, mask, &statx_buffer) < 0 )
            printf("\nFailed to stat file named '%s'\n", argv[i]);
            else
            {
                if (S_ISLNK(statx_buffer.stx_mode)) 
                {  /* File's a soft link */
                if ( report_on_link == AT_SYMLINK_NOFOLLOW) 
                {
                    /* Report is of the link itself, not its target, so
                       write the file name in the form 'link -> target' */
                    errno = 0;
                    if ( -1 == (nbytes = readlink(argv[i], target, sizeof(target))))
                        fprintf(stderr, "\nFailed to read link.");
                    else 
                    {
                        target[nbytes] = '\0';
                        printf("\nFile: %s -> %s\n", argv[i], target);
                    }
                }
                else /* Report is of the target */
                printf("\nFile: %s\n", argv[i]);
                }
            
            /*If the files are valid, else statement will run*/
            else
            {
                if(foundoperation == 'a')
                {
                    if(statx_buffer.stx_mask & STATX_ATIME)
                    {
                        struct tm *bdtime;                 /* Broken-down time                 */
                        time_t     time_val;               /* For converted tv_sec field       */

                        time_val = statx_buffer.stx_atime.tv_sec;          /* Convert to time_t           */
                        bdtime = localtime(&time_val);          /* Convert to broken-down time */
                        if (bdtime == NULL)                     /* Check for error             */
                        fprintf(stderr, "\nLocaltime failed");
                        long long int seconds = bdtime->tm_sec + bdtime->tm_min*60 + bdtime->tm_hour*3600 + bdtime->tm_yday*86400 + (bdtime->tm_year-70)*31536000 + ((bdtime->tm_year-69)/4)*86400 - ((bdtime->tm_year-1)/100)*86400 + ((bdtime->tm_year+299)/400)*86400;
                        
                        addLast(&head, argv[i], seconds);

                        if(r_option != true)
                        {   
                            printf("File: %s", argv[i]);
                            printTime("\nLast Access: ", &statx_buffer.stx_atime);
                            if ( i < argc-1 )
                            printf("----------------------------------"
                            "-----------------------------------------\n");
                        }
                    }
                        else
                        printf("\nCannot access data for '%s'", argv[i]);      
                    }
                    else if(foundoperation == 'b')
                    {
                        if(statx_buffer.stx_mask & STATX_BTIME)
                        {
                            struct tm *bdtime;                 /* Broken-down time                 */
                            time_t     time_val;               /* For converted tv_sec field       */

                            time_val = statx_buffer.stx_atime.tv_sec;          /* Convert to time_t           */
                            bdtime = localtime(&time_val);          /* Convert to broken-down time */
                            if (bdtime == NULL)                     /* Check for error             */
                            fprintf(stderr, "\nLocaltime failed");
                            long long int seconds = bdtime->tm_sec + bdtime->tm_min*60 + bdtime->tm_hour*3600 + bdtime->tm_yday*86400 + (bdtime->tm_year-70)*31536000 + ((bdtime->tm_year-69)/4)*86400 - ((bdtime->tm_year-1)/100)*86400 + ((bdtime->tm_year+299)/400)*86400;
                            addLast(&head, argv[i], seconds);

                        if(r_option != true)
                        {   
                            printf("File: %s", argv[i]);
                            printTime("\nLast Access: ", &statx_buffer.stx_btime);
                            if ( i < argc-1 )
                            printf("----------------------------------"
                            "-----------------------------------------\n");
                        }
                        }
                        else
                        fprintf(stderr, "\n'%s' is unavailable for comparison.", argv[i]);     
                    }
                    else if(foundoperation == 'c')
                    {
                        if(statx_buffer.stx_mask & STATX_CTIME)
                        {
                            struct tm *bdtime;                 /* Broken-down time                 */
                            time_t     time_val;               /* For converted tv_sec field       */

                            time_val = statx_buffer.stx_atime.tv_sec;          /* Convert to time_t           */
                            bdtime = localtime(&time_val);          /* Convert to broken-down time */
                            if (bdtime == NULL)                     /* Check for error             */
                            fprintf(stderr, "\nLocaltime failed");
                            long long int seconds = bdtime->tm_sec + bdtime->tm_min*60 + bdtime->tm_hour*3600 + bdtime->tm_yday*86400 + (bdtime->tm_year-70)*31536000 + ((bdtime->tm_year-69)/4)*86400 - ((bdtime->tm_year-1)/100)*86400 + ((bdtime->tm_year+299)/400)*86400;
                            addLast(&head, argv[i], seconds);

                            if(r_option != true)
                            {   
                                printf("File: %s", argv[i]);
                                printTime("\nLast Access: ", &statx_buffer.stx_ctime);
                                if ( i < argc-1 )
                                printf("----------------------------------"
                                "-----------------------------------------\n");
                            }
                        }
                        else
                        printf("\nCannot access data for '%s'", argv[i]);      
                    }
                    else if(foundoperation == 'm')
                    {
                    
                        struct tm *bdtime;                 /* Broken-down time                 */
                        time_t     time_val;               /* For converted tv_sec field       */

                        time_val = statx_buffer.stx_atime.tv_sec;          /* Convert to time_t           */
                        bdtime = localtime(&time_val);          /* Convert to broken-down time */
                        if (bdtime == NULL)                     /* Check for error             */
                        fprintf(stderr, "\nLocaltime failed");
                        long long int seconds = bdtime->tm_sec + bdtime->tm_min*60 + bdtime->tm_hour*3600 + bdtime->tm_yday*86400 + (bdtime->tm_year-70)*31536000 + ((bdtime->tm_year-69)/4)*86400 - ((bdtime->tm_year-1)/100)*86400 + ((bdtime->tm_year+299)/400)*86400;
                       
                        printf("\n'%s' Seconds: %d", argv[i],seconds);
                        addLast(&head, argv[i], seconds);


                        if(r_option != true)
                        {
                            printf("File: %s", argv[i]);
                            printTime("\nLast Modification: ", &statx_buffer.stx_mtime);
                            if ( i < argc-1)
                            printf("----------------------------------"
                            "-----------------------------------------\n");
                        }

                    }
                    else if(foundoperation == 's')
                    {
                        if(statx_buffer.stx_mask & STATX_SIZE)
                        {
                            addLast(&head, argv[i], statx_buffer.stx_size);
                            
                            if(r_option != true)
                            {
                                printList(head, foundoperation);
                                if ( i < argc-1 )
                                printf("----------------------------------"
                                "-----------------------------------------\n");
                            }       
                        }
                        else
                        printf("\nCannot access data for '%s'", argv[i]);
                    }
                    else if(foundoperation == 'u')
                    {
                        if (statx_buffer.stx_mask & STATX_BLOCKS)
                        {
                           addLast(&head, argv[i], statx_buffer.stx_blocks);
                            
                            if(r_option != true)
                            {
                                printList(head,foundoperation);
                                if ( i < argc-1 )
                                printf("----------------------------------"
                                "-----------------------------------------\n");
                            }       
                        }
                        else
                        printf("\nCannot access data for '%s'", argv[i]);
                    }
                
                /*If user wants link's target stat*/
        }   }
    }

        if(r_option == true)
        {
            printf("\n---Files Ordered By Largest To Smallest---\n");
            switch(foundoperation)
            {
                case 'a': 
                {
                    sortLinkedList(&head);
                    printList(head, 'a');
                    break;
                }
                case 'b': 
                {
                    sortLinkedList(&head);
                    printList(head, 'b');
                    break;
                }
                case 'c': 
                {
                    sortLinkedList(&head);
                    printList(head, 'c');
                    break;
                }
                case 'm': 
                {
                    sortLinkedList(&head);
                    printList(head, 'm');
                    break;
                }

                case 's':
                {
                    sortLinkedList(&head);
                    printList(head, 's');
                    break;
                }

                case 'u':
                {
                    sortLinkedList(&head);
                    printList(head, 'u');
                    break;
                }
            }
        }
}//main end bracket

