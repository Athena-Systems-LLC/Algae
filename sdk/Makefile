#
# ALGAE SDK Build Configuration
# Copyright (c) 2025, Ian Moffett and the Algae team.
# All rights reserved.
# Provided under the BSD 3-Clause license.
#

ARCH =
CFILES = $(shell find core/ -name "*.c")
OFILES = $(shell find platform/ -name "*.S.o")
OFILES += $(CFILES:.c=.o)
TARGET = -target x86_64-mingw64
CFLAGS = -ffreestanding -nostdlib -fno-pie -Iinclude/
CC = clang

.PHONY: all
all: platform $(OFILES)
	ar rcs alsdk.lib $(OFILES)

%.o: %.c
	$(CC) $(TARGET) -c $(CFLAGS) $< -o $@

.PHONY: platform
platform:
	cd platform/$(ARCH); make
