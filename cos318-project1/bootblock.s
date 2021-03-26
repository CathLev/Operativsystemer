# Author(s): <Your name here>
# COS 318, Fall 2013: Project 1 Bootloader
# bootloader
# Largely unimplemented

# .equ symbol, expression
# These directive set the value of the symbol to the expression
# memory constants
.equ  BOOT_SEGMENT,0x7c0
# more memory constants...

# utility constants
.equ  DISK_READ,0x02
# more utility constants...

.text               #Code segment
.globl    _start    #The entry point must be global
.code16             #Real mode

#
# The first instruction to execute in a program is called the entry
# point. The linker expects to find the entry point in the "symbol" _start
# (with underscore).
#

_start:
	mov $0x71,%al
	call print_char

# Area reserved for createimage to write the OS size
os_size:
	.word   0
	.word   0
	
# setup registers for kernel data and disk read	
load_kernel:  	

# setup the kernel stack
setup_stack:	

# switch control to kernel
switch_to_kernel:


# print a character to screen at the position of the cursor. TODO: advance the cursor
print_char:
	push %ebx
	mov $0x0e,%ah #specify teletype
	mov $0x00,%bh #page number
	mov $0x02,%bl #color of foreground (white)
	int $0x10 #call interrupt
	pop %ebx

print_string:
	