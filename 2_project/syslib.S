# syslib.S
# COS 318, Fall 2019: Project 2 Non-Preemptive Kernel
# System library containing SYSCALL macro
#
# This file replaces the original syslib.c which had a bug
# SYSCALL in syslib.c changed %eax and %eflags before calling kernel_entry

.text
.code32
.equ ENTRY_POINT, 0x0f00

#define SYSCALL(i) \
    pushl   %eax; \
    movl    ENTRY_POINT, %eax; \
    pushl   i; \
    call    *%eax; \
    popl    %eax; \
    popl    %eax

.globl yield
.globl exit

yield:
    SYSCALL($0)
    ret

exit:
    SYSCALL($1)
    ret
