include ../../data/sys.mk

SDKDIR =
OBJ = $(shell find ../../ -name "*.o")

ASMFILES = $(shell find . -name "*.S")
ASMOBJ = $(ASMFILES:.S=.S.o)

CFILES = $(shell find . -name "*.c")
COBJ = $(CFILES:.c=.o)

LD_FLAGS := -Tconf/link.ld
KERNEL_OUTPUT = ../../data/boot/algae.sys

CINC = -I../../include -I ../../target
CFLAGS = -target x86_64-elf $(MD_CFLAGS) -MMD -I$(SDKDIR)/include/
CFLAGS += $(CINC)
CC =
LD = lld.ld

.PHONY: all
all: $(ASMOBJ) $(COBJ)
	ld $(LD_FLAGS) $(OBJ) -o $(KERNEL_OUTPUT)
	../../../tools/ksyms ../../ke/ksyms.c $(KERNEL_OUTPUT)
	$(CC) -c $(CFLAGS) ../../ke/ksyms.c -o ../../ke/ksyms.o
	ld $(LD_FLAGS) $(OBJ) -o $(KERNEL_OUTPUT)

%.S.o: %.S
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(ASMOBJ) $(COBJ)
