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

.PHONY: all
all: ke platform

.PHONY: platform
platform:
	cd platform/$(ARCH); make

.PHONY: ke
ke:
	cd ke; make CC=$(CC) LD=$(LD)

.PHONY: clean
clean:
	cd platform/$(ARCH); make clean
	cd ke; make clean
