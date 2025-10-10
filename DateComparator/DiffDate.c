#define _XOPEN_SOURCE //Note: When running WSL I get "warning: implicit declaration of function ‘strptime’"
/*This is because strptime doesn't have an implementation on Windows, its only a POSIX standard,
I'm using minGW to compile, which doesn't seem to have access to strptime; might be only a Linux thing*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>  

bool validdate(int b, char *a[])
{  
   bool flag = true; //determines return value

   for (int i = 1; i < b; i++)
   {
      int year = 0, month = 0, day = 0; 
      //NOTE: orignally written "%4d-%2d-%d" but I had to change it
      //because it would allow day = 1234 and only scan the 12 and consider it valid
      
      if (sscanf(a[i], "%4d-%2d-%d", &year, &month, &day) == 3) //Checking if string format is valid
      {

      if (year>=1900 && year<=9999)
      {
         //check month
         if (month>=1 && month<=12)
         {
            //check days
            if((day>=1 && day<=31) && (month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12))
            flag = true;
               
            else if((day>=1 && day<=30) && (month==4 || month==6 || month==9 || month==11))
            flag = true;

            else if((day>=1 && day<=28) && (month==2))
            flag = true;        

            else if(day==29 && month==2 && (year%400==0 ||(year%4==0 && year%100!=0))) //leap year calculations
            flag = true;        

            else
            {
               fprintf(stderr,"'%s' does not have a valid day.\n", a[i]);
               flag = false;
            }
                  
         }
         else
         {
               fprintf(stderr,"'%s' does not have a valid month.\n", a[i]);
               flag = false;
         }
      }

      //All other cases format will be invalid
      else
      {
         fprintf(stderr,"'%s' does not have a valid year.\n", a[i]);
         flag = false;
      }
      }

      else 
      {
         fprintf(stderr, "Error: The input '%s' was not a valid time format. Format: YYYY-MM-DD.\n", a[i]);
         return false;
      } 
   }
 return flag;
}
   
void oneargumentinput(char *a[])
{
   
   //Finding current time 
   time_t date_t; //An arithmetic type capable of representing time                  
   struct tm *date; //Structure containing a calendar date and time broken down into its components
   char currenttime[50];
   time(&date_t); //Gets current time and stores into date_t
   date = localtime(&date_t); //takes time in date_t and puts it into broken down time

   // turns the time into a string and stores in currenttime
   strftime(currenttime, sizeof(currenttime), "%Y-%m-%d", date); 

   
   //Turning arg1's date into broken down time in order to compare times
   struct tm arg1 = {0};
   strptime(a[1], "%Y-%m-%d", &arg1); 
   time_t arg1_t;
   arg1_t = mktime(&arg1); 


   //Finding the time difference but for days before a year 
   double seconds = difftime(arg1_t,date_t);         
   int days = seconds / 86400; //86400 the number of seconds per day

   //Getting specific string formatting for input string
   char size[100];
   char arg1date[100];

   strftime(arg1date,sizeof(arg1date), "%Y-%m-%d", &arg1);
   strftime(size,sizeof(size),"%B %d, %Y", &arg1); //stores the string value inside size

      bool issameday = false;
      if (strcmp(currenttime, arg1date) == 0)
      {
         issameday = true;
         printf("%s is the same day as today \n", size);
      } //I had to do this because days == 0 results in today and tomorrow = same day to the program

         //Because of the 24 hour system, it considers tomorrows date as 0 days after todays date
         if(days <= 365 && issameday == false && seconds > 0)
         {
            //subtract year/month/date manually
            int yearresult = (arg1.tm_year - date->tm_year) * 365;
            int monthresult = (arg1.tm_mon - date->tm_mon) * 30;
            int dayresult = arg1.tm_mday - date->tm_mday;

            int result = yearresult + monthresult + dayresult;
            printf("\n%s is %d day(s) after today\n", size, result);   
               
         }
         
         else 
         {
            if(seconds < 0 && issameday == false)
            {
               days = abs(days); 
               printf("%s was %d day(s) before today \n", size, days);
            }
            else if (seconds > 0 && issameday == false)
            {
               printf("%s is %d day(s) after today \n", size, days);

            }
         }
      
}

void twoargumentinput(char *a[])
{
   struct tm arg2 = {0}; //Sets all struct values to 0
   struct tm arg1 = {0}; //tm must be initialized before the calling strptime

   //Changing string values in argv[1] and argv[2] into time_t 
   strptime(a[2],"%Y-%m-%d", &arg2); //Returns a broken down time structure named arg2
   strptime(a[1], "%Y-%m-%d", &arg1); //This function does not initialize tm but stores only the values specified.

   time_t arg2_t, arg1_t;

   arg2_t = mktime(&arg2); //mktime : broken down time -> time_t 
   arg1_t = mktime(&arg1);

   long double seconds = difftime(arg2_t,arg1_t); //needs a time_t format
   int days = seconds / 86400; //86400 the number of seconds per day

   //Getting specific string formatting for input string argv[2]
   char arg2_size[100];
   struct tm *arg2_tm;
   arg2_tm = localtime(&arg2_t);

   strftime(arg2_size,sizeof(arg2_size),"%B %d, %Y", arg2_tm); 


   //Getting specific string formatting for input string argv[1]
   char arg1_size[100];
   struct tm *arg1_tm;
   arg1_tm = localtime(&arg1_t);

   strftime(arg1_size,sizeof(arg1_size),"%B %d, %Y", arg1_tm); 
   
      if(days == 0)
      {
         printf("\n%s is the same %s \n", arg2_size, arg1_size);
      }

      else
      {
         if(seconds < 0)
         {
            days = abs(days);
            printf("\n%s was %d day(s) before %s \n",arg2_size, days, arg1_size);
         }
            else if (seconds > 0)
         {
            printf("\n%s is %d day(s) after %s \n",arg2_size, days, arg1_size);
         }
      }
}    

int main(int argc, char *argv[])
{ 
    setlocale(LC_ALL, ""); //Takes current system locale setting

    switch(argc) //cases are based on the number of input arguments
    { 
        case 1: 
        {
         fprintf(stderr, "Error: Not Enough Arguments.\n");
         break;
        }
        
        case 2:
        {
         if(validdate(argc, argv) == true) 
         {
           oneargumentinput(argv);
         }
         break;
        }

        case 3: 
        {
         if(validdate(argc, argv) == true)
         {
            twoargumentinput(argv);
         }
         break;
        }

        default:
        fprintf(stderr,"Error: Invalid program usage. Please enter either 1 or 2 dates to compare."
                       "Format: YYYY-MM-DD.\n");
        break;
    } 
}


/*Resources: 
https://en.cppreference.com/w/c/chrono/strftime#:~:text=Converts%20the%20date%20and%20time,to%20count%20bytes%20are%20written.
https://www.tutorialspoint.com/c_standard_library/time_h.htm
https://www.tutorialspoint.com/c_standard_library/c_function_ctime.htm
https://cplusplus.com/reference/ctime/difftime/  (Finding time difference)
https://cboard.cprogramming.com/c-programming/169114-how-convert-string-time_t.html
https://man7.org/linux/man-pages/man3/strptime.3.html
https://stackoverflow.com/questions/50614923/how-is-the-locale-of-a-c-program-set-to-the-c-locale

Understanding Time:
https://www1.udel.edu/CEND/rugg/Cfall/slides/prg17-04.html#:~:text=time_t%20An%20arithmetic%20type%20capable,called%20the%20broken%2Ddyown%20time.
https://www.gnu.org/software/libc/manual/html_node/Time-Types.html#:~:text=time_t%20is%20the%20simplest%20data,time_t%20values%20is%20not%20specified.

Understanding time_t struct:
https://en.cppreference.com/w/c/chrono/time_t

Understanding Calender Time:
https://en.cppreference.com/w/c/chrono/time
https://cplusplus.com/reference/ctime/tm/

Working with Local Time:
https://en.cppreference.com/w/c/chrono/time

Convert Seconds to Date:
https://stackoverflow.com/questions/23026148/how-to-convert-seconds-into-yearmonthday-hourminutesseconds-time

How to force strftime to use sytem locale setting:
https://stackoverflow.com/questions/27236921/strftime-force-output-in-same-local-timezone
*/

/*Note: 

time_t is a data type used by C and C++ programs to represent dates and times internally. 
time_t is actually just an integer, a whole number, that counts the number of seconds 
since January 1, 1970 at 12:00 AM Greenwich Mean Time. 
A time_t value of 0 would be 12:00:00 AM (exactly midnight) 1-Jan-1970, 
a time_t value of 1 would be 12:00:01 AM (one second after midnight) 1-Jan-1970, etc.. 


The compiler does not modify the source code and add an explicit call to setlocale() to your main program.
Rather, the implementation (the combination of compiler and library) is required to ensure 
that the program behaves 'as if' there was a call to setlocale() at the start.
*/
