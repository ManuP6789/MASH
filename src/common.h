// FIle created by Michael Kerrisk
#ifndef COMMON_H
#define COMMON_H /* Prevent accidental double inclusion */

#include <sys/types.h> /* Type definitions used by many programs */
#include <stdio.h> /* Standard I/O functions */
#include <sys/ioctl.h> /* manipulates the underlying device parameters of special files */
#include <stdlib.h> // Prototypes of commonly used library functions,
                    // plus EXIT_SUCCESS and EXIT_FAILURE constants 
#include <math.h>

#include <unistd.h> /* Prototypes for many system calls */
#include <errno.h> /* Declares errno and defines error constants */
#include <string.h> /* Commonly used string-handling functions */
#include "get_num.h" /* Declares our functions for handling numeric arguments (getInt(), getLong()) */
#include <stdbool.h> /* Declares bools for C */
#include "assert.h"

// #include "error_functions.h" /* Declares our error-handling functions */

typedef enum { FALSE, TRUE } Boolean;

#define min(m,n) ((m) < (n) ? (m) : (n)) 
#define max(m,n) ((m) > (n) ? (m) : (n))

#endif
