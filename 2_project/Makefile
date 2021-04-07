# Makefile for Project 2, COS 318

CC = gcc
LD = ld

# Addresses of the kernel, process 1, process 2, and process 3
KERNEL_ADDR = 0x1000
PROC1_ADDR = 0x10000
PROC2_ADDR = 0x20000
PROC3_ADDR = 0x30000

# Assembler flags
ASFLAGS += -m32

# Compiler flags
# No external environment
CFLAGS += -ffreestanding
# Turn on all warnings
CFLAGS += -Wall
# 32-bit compiling
CFLAGS += -m32
# Turn on optimization
CFLAGS += -O2
# Allow use of GNU's version of C99
CFLAGS += -std=gnu99
# -fstack-protector is not compatible with -ffreestanding
CFLAGS += -fno-stack-protector
# -funit-at-time reorders functions but we need the entry point to be first in
# the object file
CFLAGS += -fno-unit-at-a-time
# Specify the memory layout with macros (#define)
CFLAGS += -DKERNEL_ADDR=$(KERNEL_ADDR)
CFLAGS += -DPROC1_ADDR=$(PROC1_ADDR)
CFLAGS += -DPROC2_ADDR=$(PROC2_ADDR)
CFLAGS += -DPROC3_ADDR=$(PROC3_ADDR)

# Linker flags
# Don't use the standard startup files or libraries
LDFLAGS += -nostdlib
LDFLAGS += -melf_i386
# Specify a location on a per-target basis
LDFLAGS += -Ttext

# Create a 1.44M floppy image for Bochs
floppy.img: image
	dd if=/dev/zero of=floppy.img bs=512 count=2880
	dd if=image of=floppy.img conv=notrunc

all: floppy.img

# Create a generic image to put on a boot medium
image: createimage bootblock kernel process1 process2 process3
	./createimage --extended $(wordlist 2,$(words $^),$^)

createimage: createimage.c
	$(CC) -Wall $^ -o $@

# Put the boot block at the specified address
bootblock: LDFLAGS += 0x0000
bootblock: bootblock.o

kernel: LDFLAGS += $(KERNEL_ADDR)
# IMPORTANT: The entry point is in kernel.o, so it must come first
kernel: kernel.o entry.o lock.o queue.o scheduler.o th1.o th2.o th3.o util.o

process1 : LDFLAGS += $(PROC1_ADDR)
# IMPORTANT: The entry point is in process1.o, so it must come first
process1 : process1.o syslib.o util.o

process2 : LDFLAGS += $(PROC2_ADDR)
# IMPORTANT: The entry point is in process2.o, so it must come first
process2 : process2.o syslib.o util.o

process3 : LDFLAGS += $(PROC3_ADDR)
# IMPORTANT: The entry point is in process3.o, so it must come first
process3 : process3.o syslib.o util.o

# Override the default implicit rule to call $(LD) directly
%: %.o
	$(LD) $(LDFLAGS) $^ -o $@

hello: hello1.c hello2.S
	$(CC) -m32 $^ -o $@

# Clean up!
clean:
	rm -f floppy.img image createimage bootblock kernel process1 process2 process3 hello *.o .depend 
.PHONY: clean distclean

# Dependency management
depend: .depend
.depend:
	$(CC) -MM *.c *.S > $@
.PHONY: depend

# Inform make of the dependencies found by gcc
# The dash causes make to make .depend if it doesn't exist
-include .depend
