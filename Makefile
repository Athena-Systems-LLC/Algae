#
# ALGAE Build Configuration
# Copyright (c) 2025, Ian Moffett and the Algae team.
# All rights reserved.
# Provided under the BSD 3-Clause license.
#

ARCH = amd64
ISO_DEST = algae.iso
QEMU_FLAGS = -cdrom $(ISO_DEST) --enable-kvm -serial stdio -smp 4
SHIMDIR = ntos/shim/limine/
SDKDIR = $(shell pwd)/sdk
OMAR = tools/omar/bin/omar
CC := clang
LD := ld

.PHONY: all
all: sdk rts ntos iso

.PHONY: rts
rts:
	cd rts/; make

.PHONY: sdk
sdk:
	cd sdk/; make ARCH=$(ARCH)

.PHONY: iso
iso:
	mkdir -p iso_root/boot/
	cp ntos/data/boot/limine.conf $(SHIMDIR)/limine-bios.sys \
        $(SHIMDIR)/limine-bios-cd.bin $(SHIMDIR)/limine-uefi-cd.bin iso_root/
	cp ntos/data/boot/*.sys iso_root/boot/
	$(OMAR) -i base -o iso_root/boot/bootpack.omar
	xorriso -as mkisofs -b limine-bios-cd.bin -no-emul-boot -boot-load-size 4\
		-boot-info-table --efi-boot limine-uefi-cd.bin -efi-boot-part \
		--efi-boot-image --protective-msdos-label iso_root/ -o $(ISO_DEST) 1>/dev/null
	rm -rf iso_root

.PHONY: ntos
ntos:
	cd ntos; make ARCH=$(ARCH) CC=$(CC) LD=$(LD) SDKDIR=$(SDKDIR)

.PHONY: clean
clean:
	cd ntos; make clean ARCH=$(ARCH)

.PHONY: run
run:
	qemu-system-x86_64 $(QEMU_FLAGS)
