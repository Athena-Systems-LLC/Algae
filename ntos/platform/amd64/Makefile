SDKDIR =
OBJ = $(shell find ../../ -name "*.o")
ASMFILES = $(shell find . -name "*.asm")
ASMOBJ = $(ASMFILES:.asm=.asm.o)
LD_FLAGS := -Tconf/link.ld
KERNEL_OUTPUT = ../../data/boot/algae.sys
AS = nasm
LD = ld

.PHONY: all
all: $(ASMOBJ)
	ld $(LD_FLAGS) $(OBJ) -o $(KERNEL_OUTPUT)

%.asm.o: %.asm
	$(AS) $< -felf64 -o $@

.PHONY: clean
clean:
	rm -f $(ASMOBJ)
