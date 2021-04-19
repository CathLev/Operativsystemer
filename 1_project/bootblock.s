# Author(s): JH

# memory constants
.equ  BOOT_SEGMENT,0x7c0
.equ KERNEL_SEGMENT, 0x1000
# more memory constants...

# utility constants - is this correct?
.equ  KERNEL_DISK_SECTOR,0x02
.equ DISK_NUMBER, 0x80
.equ KERNEL_SIZE, 0x09 # is 9
# more utility constants...

# CH and CL both ff from drive paramter INT
# maximum head number 0f
# number of drives 01

.text               #Code segment
.globl    _start    #The entry point must be global
.code16             #Real mode

_start:

# Area reserved for createimage to write the OS size
os_size:
	.word   0
	.word   0

	# call print_string

# setup registers for kernel data and disk read
load_kernel:
	xchgw %bx, %bx

	mov $DISK_NUMBER, %dl # 80 -> hard drive, 0 -> floppy

	mov $0, %dh # head number ?
	mov $KERNEL_SIZE, %al # number of sectors to read from hard disk. USE os_size
	mov $os_size, %al # number of sectors to read from hard disk. USE os_size

	mov $0, %ch # cylinder nr
	mov $KERNEL_DISK_SECTOR, %cl #

	# mov $0, %ax
	# mov %ax,  %es # sector
	mov $KERNEL_SEGMENT, %bx # memory location of kernel

	# call the interrupt
	mov $0x02, %ah # tell INT 0x13 to read disk

	int $0x13

	xchgw %bx, %bx

	jc disk_error

# setup the kernel stack
setup_stack:
	mov %bp, %sp # reset stack pointer

# switch control to kernel
switch_to_kernel:
	mov $0, %ax
	# mov %ax, %cs # set these to 0 for the kernel, are already 0
	# mov %ax, %ds
	push $KERNEL_SEGMENT
	ret # jump to above adress


disk_error:
	push 'E'
	call print_char

	ret

	# Print test messages
	print_test:
		mov $'J',%ax
		push %ax
		call print_char

		mov $'G',%ax
		push %ax
		call print_char

		mov $'H',%ax
		push %ax
		call print_char

		mov $'R',%ax
		push %ax
		call print_char

# print a character to screen at the position of the cursor. TODO: advance the cursor
print_char:
	pushw %bp
	movw %sp, %bp
	# pushw %ax
	pushw %cx
	pushw %bx

	mov 4(%bp), %al # moving the char at 4(%sp) into al, adjusting for bp
	# movb $0x41, %al # just checking
	mov $0, %bh
	mov $1, %cx
	mov $0x0e, %ah
	int $0x10

	# popw %ax
	popw %bx
	popw %cx
	movw %bp, %sp
	popw %bp

	ret

print_string: # can't find the right string but otherwise should work?
	pushw %bp
	movw %sp, %bp

	# mov 4(%bp), %al # moving the char at 4(%sp) into al, adjusting for bp
	movw %cx, %si

	str_loop:
		xchgw %bx, %bx
		mov (%si), %al
		cmpb $0, %al
		jne print_next
		ret
	print_next:
		push %ax
		call print_char
		add $1, %si
		jmp str_loop
