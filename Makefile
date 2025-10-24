#
# ALGAE Build Configuration
# Copyright (c) 2025, Ian Moffett and the Algae team.
# All rights reserved.
# Provided under the BSD 3-Clause license.
#

ARCH = amd64

.PHONY: all
all: ntos

.PHONY: ntos
ntos:
	cd ntos; make ARCH=$(ARCH)

.PHONY: clean
clean:
	cd ntos; make clean ARCH=$(ARCH)
