
# Author(s): C
# COS 318, Fall 2013: Project 1 Bootloader
# bootloader
# Largely unimplemented

# .equ symbol, expression
# These directive set the value of the symbol to the expression
# memory constants
.equ  BOOT_SEGMENT,0x7c0
.equ KERNEL_SEGMENT_ADDR,0x0000
.equ KERNEL_OFFSET_ADDR,0x1000
.equ STACK_SEGMENT, 0x8900
.equ STACK_POINTER, 0xf000

# utility constants
.equ DISK_READ,0x02

.text               # Code segment
.globl    _start    # The entry point must be global
.code16             # Real mode

#
# The first instruction to execute in a program is called the entry
# point. The linker expects to find the entry point in the "symbol" _start
# (with underscore).
#

_start:

# Area reserved for createimage to write the OS size
os_size:
	.word   0
	.word   0
	
# setup registers for kernel data and disk read	
load_kernel:
	

# Read sectors into memory
# looks good so far
	read:
	xchgw %bx, %bx

	movw $KERNEL_SEGMENT_ADDR, %ax
	movw %ax, %es # Destination segment

	# get os size
	movw $BOOT_SEGMENT, %bx
	shl $0x4, %bx
	addw $os_size, %bx
	movw (%bx), %ax

	mov $DISK_READ,%ah # specify function
	movw $KERNEL_OFFSET_ADDR, %bx # Destination address
	mov $0x00, %ch # Starting cylinder number
	mov $0x02, %cl # Starting sector number
	mov $0x00, %dh # Starting head number
	# movw $0x02, %dl # Drive number (initial %dl)

	int $0x13 # Call function


	xchgw %bx, %bx

	xchgw %bx, %bx

	jc read # If error, try again
	or %ah, %ah
	jnz read # If not successful, try again
	movw 0x1000, %ax
	or %ax, %ax
	jz read # If not successful, try again

	# Set registers
	# Set data register
	movw $KERNEL_SEGMENT_ADDR, %ax
	movw %ax, %ds 
	movw %ax, %es 

	call setup_stack

# setup the kernel stack
setup_stack:
	mov $STACK_SEGMENT, %ax
	movw %ax, %ss
	mov $STACK_POINTER, %ax
	movw %ax, %sp
	
	call switch_to_kernel


# switch control to kernel
switch_to_kernel:
movw $KERNEL_OFFSET_ADDR, %ax
jmp *%ax
	jmp	$KERNEL_SEGMENT_ADDR, $KERNEL_OFFSET_ADDR
	ret	
	


# print a character to screen at the position of the cursor. TODO: advance the cursor
print_char:

    pushw %bp
    movw %sp, %bp
	push %bx
 
	# mov 4(%bp), %al
	mov $0x0e,%ah # specify teletype
	mov $0x00,%bh # page number
	mov $0x02,%bl # color of foreground (white)
	int $0x10 # call interrupt

	pop %bx
	movw %bp, %sp
	popw %bp

	ret


print_string:
    pushw %bp
    movw %sp, %bp
	push %bx
	 

	xchgw %bx, %bx

	xchgw %bx, %bx

	print_string_loop_start:
	lodsb

	or %al, %al
	call print_char

	jnz print_string_loop_start

	pop %bx
	movw %bp, %sp
	popw %bp
	ret


# print a character to screen at the position of the cursor. TODO: advance the cursor
clear_screen:

    pushw %bp
    movw %sp, %bp

	mov $0x0000, %ax
	int $0x10

	movw %bp, %sp
	popw %bp


	ret
	