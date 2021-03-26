# Makefile for Project 3, COS 318

CC = gcc
LD = ld

# Entry points of the kernel and the processes
KERNEL_ADDR = 0x1000
PROC1_ADDR = 0x10000
PROC2_ADDR = 0x20000

# Assembler flags
# 32-bit code generation
ASFLAGS += -m32

# C compiler flags
# Normally, we would select c99, but we need inline assembly
CFLAGS += -std=gnu99
# 32-bit code generation
CFLAGS += -m32
# No external environment
CFLAGS += -ffreestanding
# Turn on all warnings and make them errors
CFLAGS += -Wall -Wextra 
# Turn on optimization
CFLAGS += -O2 -fomit-frame-pointer -fno-builtin
# -fstack-protector is not compatible with -ffreestanding
CFLAGS += -fno-stack-protector
# -funit-at-time reorders functions but we need the entry point to be first in
# the object file
CFLAGS += -fno-unit-at-a-time
# 4-byte stack alignment
# The default of a 64-bit compiler is to align by 16
CFLAGS += -mpreferred-stack-boundary=2
# Specify the memory layout with macros (#define)
CFLAGS += -DKERNEL_ADDR=$(KERNEL_ADDR)
CFLAGS += -DPROC1_ADDR=$(PROC1_ADDR)
CFLAGS += -DPROC2_ADDR=$(PROC2_ADDR)
# Add debugging symbols
CFLAGS += -g -c

# Linker flags
# 32-bit code generation
LDFLAGS += -melf_i386
# Don't use the standard startup files or libraries
LDFLAGS += -nostdlib
# Specify a location on a per-target basis
LDFLAGS += -Ttext

# Create a generic image to put on a boot medium
all: kernel process1 process2
	./createimage --extended bootblock kernel process1 process2

createimage: createimage.c
	$(CC) -Wall $^ -o $@

# Put the boot block at the specified address
bootblock: LDFLAGS += 0x0000
bootblock: bootblock.o

kernel: LDFLAGS += $(KERNEL_ADDR)
# IMPORTANT: The entry point is in kernel.o, so it must come first
kernel: kernel.o entry.o interrupt.o queue.o scheduler.o sync.o util.o printf.o barrier_test.o philosophers.o th1.o th2.o

kernel_test: LDFLAGS += $(KERNEL_ADDR)
# IMPORTANT: The entry point is in kernel.o, so it must come first
kernel_test: kernel.o entry.o interrupt.o queue.o scheduler.o sync.o util.o printf.o th1.o th2.o

process1: LDFLAGS += $(PROC1_ADDR)
# IMPORTANT: the entry point is in process1.o, so it must come first
process1: process1.o syslib.o util.o printf.o

process2: LDFLAGS += $(PROC2_ADDR)
# IMPORTANT: The entry point is in process2.o, so it must come first
process2: process2.o syslib.o util.o printf.o helper.o

# Compile all C files
%.o: %.c
	$(CC) $(CFLAGS) $<

# Override the default implicit rule to call $(LD) directly
%: %.o
	$(LD) $(LDFLAGS) $^ -o $@

# Clean up!
clean:
	rm -f image kernel process1 process2 *.o .depend serial.out log.txt bochsout.txt

cleantest: clean
	rm -f process*.* philosophers.* barrier_test.* tasks.* th*.*
.PHONY: boot depend clean distclean

# Dependency management
depend: .depend
.depend:
	$(CC) -MM *.c *.S > $@

# Inform make of the dependencies found by gcc
# The dash causes make to make .depend if it doesn't exist
-include .depend
