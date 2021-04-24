# Author(s): JH

# memory constants
.equ BOOT_ADDRESS,0x7c00
.equ NEW_BOOT_SEGMENT, 0x7e0
.equ NEW_BOOT_ADDRESS, 0x0000
.equ KERNEL_ADDRESS, 0x1000
.equ STACK_SEGMENT, 0x9000
.equ STACK_INITIAL_OFFSET, 0xf000

# utility constants
.equ BOOTLOADER_DISK_SECTOR, 0x01 # bootloader is in first sector
.equ KERNEL_DISK_SECTOR,0x02 # the kernel is located in the second sector
.equ DISK_NUMBER, 0x80 # hard drive number 1

.text               # Code segment
.globl    _start    # The entry point must be global
.code16             # Real mode

_start:

# Area reserved for createimage to write the OS size
os_size:
	.word   0
	.word 	0


# setup registers for kernel data and disk read
move_bootloader:
	xchgw %bx, %bx

	# Set up the INT 0x13 call to read disk
	mov $DISK_NUMBER, %dl 	# The disk to read
	mov $0, %dh 						# head number ?

	# Bootloader is 1 sector
	mov $1, %al 
	
	# moving on
	mov $0, %ch 						# cylinder nr ?
	mov $BOOTLOADER_DISK_SECTOR, %cl # The sector to start looking in
	mov $NEW_BOOT_SEGMENT, %bx
	mov %bx,  %es 					# segment
	mov $NEW_BOOT_ADDRESS, %bx # memory location of kernel
	mov $0x02, %ah # tell INT 0x13 to read disk

	pusha
	int $0x13
	jc disk_error
	popa

	xchgw %bx, %bx
	
	# transfer control to new bootloader
	mov $NEW_BOOT_SEGMENT, %ax
	imul $0x10, %ax
	add $NEW_BOOT_ADDRESS, %ax
	add $load_kernel, %ax
	jmp *%ax



# setup registers for kernel data and disk read
load_kernel:

	# Set up the INT 0x13 call to read disk
	mov $DISK_NUMBER, %dl 	# The disk to read
	mov $0, %dh 						# head number ?

	# Now we read the kernel size from createimage placed at relative adress 0x02
	mov $BOOT_ADDRESS, %bx
	add $os_size, %bx
	add $2, %bx
	mov (%bx), %al 
	
	# moving on
	mov $0, %ch 						# cylinder nr ?
	mov $KERNEL_DISK_SECTOR, %cl # The sector to start looking in
	mov $0, %bx
	mov %bx,  %es 					# sector ?
	mov $KERNEL_ADDRESS, %bx # memory location of kernel
	mov $0x02, %ah # tell INT 0x13 to read disk

	int $0x13
	jc disk_error

	xchgw %bx, %bx

# setup the kernel stack
setup_stack:
	# Set stack to use 64KB range 0x90000 - 0xa0000. need 10 more bits, or crash
	mov $STACK_SEGMENT, %ax
	mov %ax, %ss
	mov $STACK_INITIAL_OFFSET, %bp
	mov %bp, %sp

# switch control to kernel
switch_to_kernel:
	# mov $0, %ax
	# mov %ax, %cs # set these to 0 for the kernel, are already 0
	# mov %ax, %ds
	# mov %ax, %ds # move data segment together with stack segment
	mov $KERNEL_ADDRESS, %ax
	# movw    %ax, %es
	# movw    %ax, %fs
	# movw    %ax, %gs
	jmp *%ax

disk_error:
	push 'E'
	call print_char

	ret

# print a character to screen at the position of the cursor.
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
