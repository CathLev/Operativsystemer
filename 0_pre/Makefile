
# Makefile for the OS projects.
# Best viewed with tabs set to 4 spaces.

CC = gcc
LD = ld

SHELL_C_OBJS = shell.o history.o allocator.o common.o
SHELL_AS_OBJS = helper.o

# Where to locate the shell in memory (not taking into
# account segments):
SHELL_ADDR	= 0x0000

# Compiler flags
#-fno-builtin:		Don't recognize builtin functions that do not begin
#			with '__builtin_' as prefix.
#
#-fomit-frame-pointer:	Don't keep the frame pointer in a register for 
#			functions that don't need one.
#
#-make-program-do-what-i-want-it-to-do:
#			Turn on all friendly compiler flags.
#
#-O2:			Turn on all optional optimizations except for loop
#			unrolling and function inlining.
#
#-c:			Compile or assemble the source files, but do not link.
#
#-Wall:			All of the `-W' options combined (all warnings on)

CCOPTS = -Wall -g -march=i386 -m32 -c -fomit-frame-pointer -fno-builtin

# Linker flags
#-nostartfiles:	Do not use the standard system startup files when linking.
#
#-nostdlib:	Don't use the standard system libraries and startup files
#		when linking. Only the files you specify will be passed
#		to the linker.
#          

LDOPTS = -nostartfiles -nostdlib -melf_i386

# Makefile targets

all: bootblock shell image

shell: $(SHELL_AS_OBJS) $(SHELL_C_OBJS)
	$(LD) $(LDOPTS) -Ttext $(SHELL_ADDR) -o shell $^
	objcopy $@ $@ -G init

bootblock: given/bootblock.o shell
	$(LD) $(LDOPTS) -Ttext 0x0 -Rshell -o bootblock $<

# Create an image to put on the floppy
image: bootblock shell
	./given/createimage --extended ./bootblock ./shell

# Put the image on the usb stick (these two stages are independent, as both
# vmware and bochs can run using only the image file stored on the harddisk)
#
# NB! You may have to change the output file (of=/dev/your_device). To find 
# your usb drive, there are two methods:
# 
# 1) insert your usb flash drive, then type 'dmesg' in bash
# 2) with your usb drive inserted, have a look at /dev/disk/by-id, it will 
# 	 list the usb drives connected as a number of links. use 'readlink' to
# 	 determine the file in /dev corresponds to your device.
boot: image
	dd if=./image of=/dev/your_device bs=512

# Clean up!
clean:
	rm -f *.o
	rm -f image image.lock bootblock shell

# No, really, clean up!
distclean: clean
	rm -f *~
	rm -f \#*
	rm -f *.bak
	rm -f serial.out
	rm -f bochsout.txt

# How to compile a C file
%.o:%.c
	$(CC) $(CCOPTS) $<

# How to assemble
%.o:%.s
	$(CC) $(CCOPTS) $<

# How to produce assembler input from a C file
%.s:%.c
	$(CC) $(CCOPTS) -S $<
