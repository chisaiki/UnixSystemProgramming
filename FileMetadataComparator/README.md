# File Metadata Comparator

## High Level Overview
This program is a Unix/Linux file metadata analysis and comparison utility that uses the advanced `statx()` system call to examine and sort files based on various metadata criteria including timestamps, file sizes, and disk usage. It provides detailed file information with flexible sorting and display options.

## Features
- **Multiple Comparison Criteria**: Sort by access time, birth time, modification time, status change time, apparent size, or disk usage
- **Advanced File Statistics**: Uses `statx()` system call for comprehensive metadata access
- **Flexible Sorting**: Largest to smallest ordering with `-r` option
- **Symbolic Link Handling**: Option to follow or report on symbolic links directly
- **Detailed Output**: Formatted display with timestamps, sizes, and file information
- **Cross-Platform Support**: Designed for modern Linux systems with `statx()` support
- **Robust Validation**: Comprehensive option parsing and error handling

## Video Tutorial
[Add video tutorial link here when available]

## Usage
```bash
./filecompare [OPTIONS] FILE1 [FILE2 ...]
```

### Options
- **`-a`** : Compare by access time (last time file was read)
- **`-b`** : Compare by birth time (file creation time)
- **`-c`** : Compare by status change time (metadata change)
- **`-m`** : Compare by modification time (last content change)
- **`-s`** : Compare by apparent size (file size in bytes)
- **`-u`** : Compare by disk usage (actual blocks used)
- **`-r`** : Sort results (largest to smallest)
- **`-l`** : Follow symbolic links to their targets

### Example Usage
```bash
./filecompare -m file1.txt file2.txt file3.txt    # Compare by modification time
./filecompare -s -r *.log                         # Sort log files by size
./filecompare -u -r /home/user/*                  # Sort by disk usage
./filecompare -a -l symlink.txt regular.txt       # Compare access times, follow links
```

### Example Output
```bash
# Without -r flag (immediate display):
File: document.txt
Last Modification: 2025-10-09 14:30:45.123456789 -0400
----------------------------------------------------------

File: image.jpg
Last Modification: 2025-10-08 09:15:22.987654321 -0400
----------------------------------------------------------

# With -r flag (sorted display):
---Files Ordered By Largest To Smallest---

#1.Filename: image.jpg
#2.Filename: document.txt
#3.Filename: script.sh
```

## Technical Implementation

### Key Components
1. **Advanced File Statistics**: Uses `statx()` for comprehensive metadata access
2. **Linked List Management**: Dynamic storage for file metadata and sorting
3. **Multiple Sort Criteria**: Timestamp conversion and size-based comparisons
4. **Symbolic Link Resolution**: Configurable link following behavior
5. **Time Conversion Engine**: Unix timestamp to human-readable formatting
6. **Option Validation**: Ensures only one comparison operation per execution

### System Calls & Libraries Used
- **`statx()`**: Advanced file metadata retrieval (Linux-specific)
- **`readlink()`**: Symbolic link target resolution
- **`localtime()`**: Time structure conversion
- **`strftime()`**: Human-readable time formatting
- **`getopt()`**: Command-line option parsing
- **`malloc()`/`free()`**: Dynamic memory management

### Metadata Types Handled
- **`STATX_ATIME`**: Access time (when file was last read)
- **`STATX_BTIME`**: Birth time (file creation - filesystem dependent)
- **`STATX_CTIME`**: Status change time (metadata modifications)
- **`STATX_MTIME`**: Modification time (content changes)
- **`STATX_SIZE`**: File size in bytes
- **`STATX_BLOCKS`**: Disk blocks allocated

## Algorithm Flow

### Processing Pipeline
1. **Option Validation**: Ensure exactly one comparison operation specified
2. **Command-line Parsing**: Process options and file arguments using `getopt()`
3. **File Processing Loop**:
   - Call `statx()` for each file
   - Handle symbolic links based on `-l` flag
   - Extract relevant metadata based on operation type
   - Convert timestamps to comparable format
   - Store in linked list or display immediately
4. **Optional Sorting**: If `-r` flag present, sort linked list by metadata values
5. **Output Generation**: Display results in requested format

### Time Conversion Process
```c
// Convert statx timestamp to seconds since epoch
time_val = statx_buffer.stx_[time_field].tv_sec;
bdtime = localtime(&time_val);
seconds = complex_calculation_including_leap_years();
```

### Sorting Algorithm
- **Bubble Sort Implementation**: Compares metadata values in linked list
- **Descending Order**: Largest values first
- **Stable Sort**: Maintains relative order for equal values

## Error Handling
- **Invalid Option Combinations**: Prevents multiple comparison operations
- **File Access Errors**: Handles permission denied and missing files
- **Symbolic Link Errors**: Graceful handling of broken links
- **Memory Allocation**: Checks for malloc failures
- **Metadata Unavailability**: Reports when specific metadata isn't available
- **Invalid Arguments**: Comprehensive usage error messages

## Platform Requirements
- **Linux Kernel**: 4.11+ (for full `statx()` support)
- **glibc**: 2.28+ (for `statx()` function exposure)
- **Compiler**: GCC with `_GNU_SOURCE` support
- **Filesystem**: Extended attribute support for birth time

## Topics Covered
- **Advanced Systems Programming**: Modern Linux system call usage
- **File System Internals**: Understanding inode metadata and timestamps
- **Data Structure Implementation**: Custom linked list with sorting
- **Memory Management**: Dynamic allocation and proper cleanup
- **Command-line Interface**: Professional option parsing with `getopt()`
- **Error Handling**: Robust system programming practices
- **Time Manipulation**: Unix timestamp conversion and formatting
- **Symbolic Link Handling**: Link resolution and target following

## Compilation
```bash
gcc -o filecompare filecompare.c -D_GNU_SOURCE
```

## Learning Objectives
- Understanding modern Linux file system interfaces
- Working with advanced system calls beyond basic `stat()`
- Implementing flexible sorting and comparison algorithms
- Managing complex command-line option combinations
- Developing robust file processing utilities
- Handling cross-filesystem metadata differences
- Building professional-grade system administration tools

## Advanced Features Demonstrated
- **`statx()` Mastery**: Using Linux's most advanced file stat interface
- **Metadata Awareness**: Understanding different timestamp meanings
- **Link Resolution**: Sophisticated symbolic link handling
- **Performance Considerations**: Efficient file processing for large datasets
- **Error Resilience**: Graceful degradation when metadata unavailable

## Known Limitations
- **Linux-Specific**: Requires `statx()` system call (Linux 4.11+)
- **Birth Time Support**: Depends on filesystem (ext4, btrfs, etc.)
- **Memory Usage**: Stores all file metadata in memory for sorting
- **Large File Sets**: No pagination for extremely large file collections

---

*This project demonstrates advanced Linux systems programming with modern file system interfaces, professional command-line tool design, and sophisticated metadata analysis capabilities.*
