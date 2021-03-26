/* syslib.h
 * COS 318, Fall 2019: Project 3 Preemptive Scheduler
 * Definitions and types for the system call library
 */

/* DO NOT CHANGE THIS FILE */

#ifndef SYSLIB_H
#define SYSLIB_H

#include "common.h"

enum {
    IGNORE = 0
};

// Yield the current process
void yield(void);

// Terminate the current process
void exit(void);

// Get the process ID
int getpid(void);

// Sleep for the specified number of milliseconds
void sleep(int);

// Shut down the computer
void shutdown(void);

// Write the character to the serial port
// Note that Bochs is configured so that every character written to the serial
// port is appended to the file serial.out
void write_serial(char);

// Reserved for Extra Credit
// Get the process priority
int getpriority(void);

// Reserved for Extra Credit
// Set the process priority (1 = lowest priority, >1 = higher priority)
void setpriority(int);

#endif
