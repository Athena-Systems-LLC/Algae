#
# ALGAE SDK Build Configuration
# Copyright (c) 2025, Ian Moffett and the Algae team.
# All rights reserved.
# Provided under the BSD 3-Clause license.
#

ASMFILES = $(shell find . -name "*.S")
TARGET = -target x86_64-mingw64
OFILES = $(ASMFILES:.S=.S.o)
CFLAGS = -ffreestanding -nostdlib -fno-pie
CC = clang

.PHONY: all
all: $(OFILES)

%.S.o: %.S
	$(CC) -c $(TARGET) $(CFLAGS) $< -o $@
