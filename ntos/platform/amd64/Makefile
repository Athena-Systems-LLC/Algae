include ../../data/sys.mk

SDKDIR =
OBJ = $(shell find ../../ -name "*.o")

ASMFILES = $(shell find . -name "*.asm")
ASMOBJ = $(ASMFILES:.asm=.asm.o)

CFILES = $(shell find . -name "*.c")
COBJ = $(CFILES:.c=.o)

LD_FLAGS := -Tconf/link.ld
KERNEL_OUTPUT = ../../data/boot/algae.sys

CFLAGS = -target x86_64-elf $(MD_CFLAGS) -MMD -I$(SDKDIR)/include/
CFLAGS += -I../../include/ -I../../target/
CC =
AS = nasm
LD = ld

.PHONY: all
all: $(ASMOBJ) $(COBJ)
	ld $(LD_FLAGS) $(OBJ) -o $(KERNEL_OUTPUT)

%.asm.o: %.asm
	$(AS) $< -felf64 -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(ASMOBJ)
