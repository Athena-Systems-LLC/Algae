#
# ALGAE Build Configuration
# Copyright (c) 2025, Ian Moffett and the Algae team.
# All rights reserved.
# Provided under the BSD 3-Clause license.
#

ARCH = amd64
ISO_DEST = algae.iso
QEMU_FLAGS = -cdrom $(ISO_DEST) --enable-kvm

.PHONY: all
all: ntos iso

.PHONY: iso
iso:
	grub-mkrescue -o $(ISO_DEST) ntos/data/

.PHONY: ntos
ntos:
	cd ntos; make ARCH=$(ARCH)

.PHONY: clean
clean:
	cd ntos; make clean ARCH=$(ARCH)

.PHONY: run
run:
	qemu-system-x86_64 $(QEMU_FLAGS)
