# User Session Tracking System

## High Level Overview
This program is a Unix/Linux user session tracking utility that reads the system's `wtmp` log file to display user login and logout information, similar to the `last` command. It tracks user sessions, system boot/shutdown events, and calculates session durations.

## Features
- **User Session Tracking**: Displays login/logout times for all users
- **Session Duration**: Calculates and displays how long each session lasted
- **System Events**: Optional display of boot/shutdown events with `-x` flag
- **Reverse Chronological Order**: Shows most recent sessions first
- **Session Status**: Identifies users still logged in, abnormal logouts, and system crashes

## Video Tutorial
[Add your video tutorial link here when available]

## Usage
```bash
./logtime [-x]
```

### Options
- `-x` : Include system events (boot/shutdown) in the output

### Output Format
```
username terminal    hostname           login_time - logout_time (duration)
```

### Example Output
```
john     pts/0        192.168.1.100     Mon Oct 09 14:30 - 16:45  (02:15)
alice    tty1         localhost         Mon Oct 09 09:00 - still logged in
bob      pts/2        192.168.1.50      Sun Oct 08 22:30 - gone - no logout
system   boot         system boot       Sun Oct 08 08:00 - 23:45  (15:45)
```

## Technical Implementation

### Key Components
1. **WTMP File Processing**: Reads `/var/log/wtmp` or system equivalent
2. **Reverse File Reading**: Processes log entries from newest to oldest
3. **Record Type Handling**: Manages different `utmpx` record types:
   - `USER_PROCESS`: User login events
   - `DEAD_PROCESS`: User logout events  
   - `BOOT_TIME`: System startup
   - `SHUTDOWN_TIME`: System shutdown
4. **Linked List Management**: Maintains logout records to match with logins
5. **Time Calculations**: Formats session durations in `(days+HH:MM)` format

### System Calls & Libraries Used
- **`utmpx.h`**: User accounting database structures
- **`open()`**: File descriptor management
- **`read()`**: Raw file reading
- **`lseek()`**: File positioning for reverse reading
- **`localtime()`**: Time conversion
- **`strftime()`**: Time formatting

### Data Structures
- **Custom Linked List**: Stores logout records for session matching
- **utmpx Structure**: Standard Unix user accounting record format

## Algorithm Flow
1. Open wtmp file in read-only mode
2. Parse command-line options
3. Read file from end to beginning (reverse chronological)
4. For each record:
   - **Login Record**: Find matching logout in saved list
   - **Logout Record**: Save to list for future login matching
   - **Boot/Shutdown**: Handle system events if `-x` enabled
5. Calculate and display session duration
6. Handle edge cases (still logged in, crashed sessions)

## Error Handling
- File access permissions
- Corrupted wtmp records
- Memory allocation failures
- Time conversion errors
- Invalid command-line options


## Topics Covered
- **Systems Programming**: Low-level file I/O operations
- **Unix User Accounting**: Understanding wtmp/utmpx log format
- **File Processing**: Reading binary files in reverse order
- **Data Structure Management**: Linked list implementation
- **Time Manipulation**: Unix timestamp conversion and formatting
- **Command-line Interface**: Option parsing with `getopt()`
- **Memory Management**: Dynamic allocation and cleanup
- **Error Handling**: Robust system programming practices

## Compilation
```bash
gcc -o logtime logtime.c -D_GNU_SOURCE
```

## Learning Objectives
- Understanding Unix logging mechanisms
- Working with binary file formats
- Implementing reverse file reading
- Managing complex data relationships (login/logout pairing)
- System-level time handling
- Robust error checking in systems programming

---

*This project demonstrates advanced Unix systems programming concepts including file I/O, user accounting, and session management.*