#include <string>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <sys/stat.h>

#include "utils.h"

const char* CHECKED = "_CHECKED_";
const char* UNCHECKED = "UNCHECKED";

const int SIG_LEN = sizeof(UNCHECKED);
const int INT_SIZE = 4;
const int LONG_SIZE = 4;

const int MAX_PATH_SIZE = 2048;

std::string get_temp_dir() {
    TCHAR buffer [MAX_PATH_SIZE];

    if (GetTempPath (MAX_PATH, buffer) != 0)
        return std::string(buffer);
    return "";
}


std::string get_absolute_path(char* arg) {
    DWORD   retval=0;
    BOOL    success; 
    char    buffer[MAX_PATH_SIZE]=TEXT(""); 
    TCHAR   buf[MAX_PATH_SIZE]=TEXT(""); 
    TCHAR** lppPart={NULL};

// Retrieve the full path name for a file. 
// The file does not need to exist.

    retval = GetFullPathName(arg, MAX_PATH_SIZE, buffer, lppPart);
    
    if (retval == 0) {
        // Handle an error condition.
        printf ("GetFullPathName failed (%d)\n", GetLastError());
        return "";
    }
    else {
        // printf("The full path name is:  %s\n", buffer);
        if (lppPart != NULL && *lppPart != 0) {
            _tprintf(TEXT("The final component in the path name is:  %s\n"), *lppPart);
        }
    }

	return std::string(buffer);
}

long get_file_size(char* filename) {
    struct stat stat_buf;
    int rc = stat(filename, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}
