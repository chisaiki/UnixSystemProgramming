# Unix System Programming

> Collection of system-level utilities demonstrating advanced Unix/Linux programming concepts.

## Quick Overview

| Program | Purpose | Key Features |
|---------|---------|--------------|
| **[UserSessionTrackingSystem](./UserSessionTrackingSystem/)** | Track user login/logout sessions | `wtmp` parsing, session duration, reverse chronological |
| **[FileMetadataComparator](./FileMetadataComparator/)** | Compare files by metadata | `statx()` system call, multiple sort criteria, symbolic links |
| **[DateComparator](./DateComparator/)** | Calculate date differences | Date validation, leap years, cross-platform compatibility |


## Technologies Demonstrated
- **System Calls**: `statx()`, `utmpx`, `time()`, `strptime()`
- **File I/O**: Binary file processing, reverse reading
- **Data Structures**: Linked lists, sorting algorithms
- **Cross-Platform**: POSIX compliance, Windows compatibility
- **Memory Management**: Dynamic allocation, proper cleanup

## Build & Run
```bash
# Compile any program
cd [ProgramDirectory]
gcc -o program program.c -D_GNU_SOURCE

# Run with appropriate arguments
./program [options] [files/dates]
```

---
*Advanced Unix systems programming projects showcasing file systems, user accounting, and time manipulation.* 

