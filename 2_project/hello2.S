# hello2.S
# COS 318, Fall 2019: Project 2 Non-Preemptive Kernel
# Example calling x86 assembly from C and vice versa

.data
format_string:
    .asciz  "value = %d\n"
ecx_save:
    .long   0

.text
.code32
# int timestwo(int n);
.globl timestwo
timestwo:
    # Prologue
    pushl   %ebp
    movl    %esp, %ebp
    movl    %ecx, ecx_save
    # printf(format_string, value);
    movl    value, %ecx
    pushl   %ecx
    pushl   $format_string
    call    printf
    # return n+n;
    # The return value goes into %eax
    movl    8(%ebp), %ecx
    addl    %ecx, %ecx
    movl    %ecx, %eax
    # Epilogue
    movl    ecx_save, %ecx
    leave
    ret
