# Date Comparator

## High Level Overview
This program is a Unix/Linux date comparison utility that calculates the difference between dates in days. It can compare a given date with today's date, or compare two user-provided dates, displaying results in human-readable format with proper date validation.

## Features
- **Date Validation**: Comprehensive validation including leap year calculations
- **Flexible Input**: Accepts 1 or 2 dates for comparison
- **Current Date Comparison**: Compare any date with today's date
- **Two-Date Comparison**: Calculate difference between any two dates
- **Human-Readable Output**: Displays dates in full format (e.g., "December 25, 2025")
- **Duration Calculation**: Shows differences in days with directional context
- **Leap Year Support**: Accurate February 29th validation
- **Cross-Platform Compatibility**: Handles Windows/WSL differences

## Video Tutorial
[Add video tutorial link here when available]

## Usage
```bash
./diffdate YYYY-MM-DD [YYYY-MM-DD]
```

### Input Format
- **Date Format**: `YYYY-MM-DD` (ISO 8601 standard)
- **Year Range**: 1900-9999
- **Single Date**: Compare with today's date
- **Two Dates**: Compare first date with second date

### Example Usage
```bash
./diffdate 2025-12-25                    # Days until Christmas
./diffdate 2020-01-01 2025-01-01        # 5 years difference
./diffdate 1995-06-15                    # Days since birth date
```

### Example Output
```
December 25, 2025 is 77 day(s) after today

January 1, 2025 is 1826 day(s) after January 1, 2020

June 15, 1995 was 11064 day(s) before today

December 25, 2025 is the same day as today
```

## Technical Implementation

### Key Components
1. **Date Validation Engine**: Comprehensive validation with leap year logic
2. **String Parsing**: Uses `sscanf()` for precise date format parsing
3. **Time Structure Management**: Converts between string, `struct tm`, and `time_t`
4. **Current Time Handling**: System time integration with `time()` and `localtime()`
5. **Duration Calculations**: Multiple algorithms for different time ranges
6. **Locale Support**: Proper date formatting using system locale

### System Calls & Libraries Used
- **`time.h`**: Core time manipulation functions
- **`strptime()`**: String to time structure conversion (POSIX)
- **`mktime()`**: Time structure to timestamp conversion
- **`difftime()`**: Precise time difference calculation
- **`strftime()`**: Formatted time string output
- **`setlocale()`**: Locale-aware date formatting
- **`sscanf()`**: Structured string parsing

### Data Structures
- **`struct tm`**: Broken-down time representation
- **`time_t`**: Unix timestamp (seconds since epoch)
- **Custom validation logic**: Day/month/year range checking

## Algorithm Flow

### Single Date Mode
1. Parse and validate input date
2. Get current system time
3. Convert input to `time_t` format
4. Calculate difference using `difftime()`
5. Handle special cases (same day, future/past)
6. Format and display human-readable output

### Two Date Mode
1. Parse and validate both input dates
2. Convert both to `time_t` format
3. Calculate difference between the two dates
4. Determine chronological relationship
5. Format both dates for display
6. Output comparison result

### Date Validation Process
1. **Format Check**: Verify `YYYY-MM-DD` pattern with `sscanf()`
2. **Range Validation**: 
   - Year: 1900-9999
   - Month: 1-12
   - Day: Context-dependent validation
3. **Month-Specific Day Validation**:
   - 31-day months: January, March, May, July, August, October, December
   - 30-day months: April, June, September, November
   - February: 28 days (29 in leap years)
4. **Leap Year Calculation**: `(year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)`

## Error Handling
- **Invalid Format**: Non-conforming date strings
- **Out-of-Range Values**: Invalid years, months, or days
- **Invalid Days**: Context-specific validation (e.g., February 30th)
- **System Time Errors**: `localtime()` and `mktime()` failures
- **Insufficient Arguments**: Command-line usage errors
- **Platform Compatibility**: Windows vs. POSIX function availability

## Platform Considerations
- **Windows/MinGW**: `strptime()` function not available
- **WSL/Linux**: Full POSIX compliance
- **Locale Support**: System-dependent date formatting
- **Compiler Flags**: Requires `_XOPEN_SOURCE` for POSIX functions

## Topics Covered
- **Systems Programming**: Low-level time manipulation
- **Date/Time Algorithms**: Leap year calculations and validation
- **String Processing**: Structured parsing with `sscanf()`
- **Unix Time Concepts**: Epoch time, broken-down time, time zones
- **Cross-Platform Development**: Handling OS-specific limitations
- **Input Validation**: Robust user input checking
- **Command-line Interface**: Argument parsing and validation
- **Locale Programming**: International date formatting
- **Error Handling**: Comprehensive edge case management

## Compilation
```bash
# Linux/WSL
gcc -o diffdate DiffDate.c -D_XOPEN_SOURCE

# Windows (MinGW) - limited functionality
gcc -o diffdate.exe DiffDate.c
```

## Learning Objectives
- Understanding Unix time representation and conversion
- Implementing comprehensive input validation
- Working with POSIX time functions
- Handling cross-platform compatibility issues
- Managing complex date/time calculations
- Developing robust command-line utilities
- Applying locale-aware programming practices

## Known Issues & Limitations
- **Windows Compatibility**: `strptime()` unavailable in MinGW
- **Time Zone Handling**: Uses local time only
- **Year 2038 Problem**: Limited by 32-bit `time_t` on some systems
- **Locale Dependency**: Output format depends on system locale

---

*This project demonstrates fundamental systems programming concepts including time manipulation, input validation, and cross-platform development challenges.*