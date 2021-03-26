# helper.S
# COS 318, Fall 2019: Project 3 Preemptive Scheduler
# Helper functions used in some of the tests

# DO NOT CHANGE THIS FILE

.text
.code32
.globl  dirty_yield
dirty_yield:
    pushfl
    pushal
    movl    44(%esp), %eax
    movl    %eax, %ecx
    movl    %eax, %edx
    movl    %eax, %ebx
    movl    %eax, %ebp
    movl    %eax, %esi
    movl    %eax, %edi
    pushl   48(%esp)
    popfl
    pushl   $0
    call    *0xf00
    movl    %esp, (esp_save)
    movl    44(%esp), %esp
    pushfl
    pushal
    movl    (esp_save), %esp
    popl    %eax
    popal
    popfl
    ret
esp_save:
    .long  0
