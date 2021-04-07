/* syslib.c
 * COS 318, Fall 2019: Project 3 Preemptive Scheduler
 * Implementation of the system call library for user processes
 * Each function implements a trap to the kernel
 */

/* DO NOT CHANGE THIS FILE */

#include "common.h"
#include "syslib.h"
#include "util.h"

// 1. Place system call number (i) in eax and arg1 in ebx
// 2. Trigger interrupt 48 (system call)
// 3. Return value is in eax after returning from interrupt
static int invoke_syscall(int i, int arg1) {
    int ret;
    asm volatile ("int $48"  // 48 = 0x30
                  :"=a" (ret)
                  :"a"(i), "b"(arg1));
    return ret;
}

void yield(void) {
    invoke_syscall(SYSCALL_YIELD, IGNORE);
}

void exit(void) {
    invoke_syscall(SYSCALL_EXIT, IGNORE);
}

int getpid(void) {
    return invoke_syscall(SYSCALL_GETPID, IGNORE);
}

void sleep(int milliseconds) {
    invoke_syscall(SYSCALL_SLEEP, milliseconds);
}

void shutdown(void) {
    invoke_syscall(SYSCALL_SHUTDOWN, IGNORE);
}

void write_serial(char c) {
    invoke_syscall(SYSCALL_WRITE_SERIAL, (int)c);
}

// Reserved for Extra Credit
int getpriority(void) {
    return invoke_syscall(SYSCALL_GETPRIORITY, IGNORE);
}

// Reserved for Extra Credit
void setpriority(int p) {
    invoke_syscall(SYSCALL_SETPRIORITY, p);
}
