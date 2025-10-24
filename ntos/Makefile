#
# ALGAE Kernel Build Configuration
# Copyright (c) 2025, Ian Moffett and the Algae team.
# All rights reserved.
# Provided under the BSD 3-Clause license.
#

include data/sys.mk

ARCH =
KERNEL_OUT = algae.sys
CC =
LD =
SDKDIR =

.PHONY: all
all: target ke platform
	rm -rf target/

.PHONY: platform
platform:
	cd platform/$(ARCH); make SDKDIR=$(SDKDIR)

.PHONY: ke
ke:
	cd ke; make CC=$(CC) LD=$(LD) SDKDIR=$(SDKDIR)

.PHONY: target
target:
	mkdir -p target/md/
	rsync -avr include/platform/$(ARCH)/* target/md/

.PHONY: clean
clean:
	cd platform/$(ARCH); make clean
	cd ke; make clean
