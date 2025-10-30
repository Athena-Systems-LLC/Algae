#
# ALGAE Client Server Runtime Service Build Script
# Copyright (c) 2025, Ian Moffett and the Algae team.
# All rights reserved.
# Provided under the BSD 3-Clause license.
#

CFILES = $(shell find . -name "*.c")
OFILES = $(CFILES:.c=.o)
CC = clang

CFLAGS = -nostdlib -ffreestanding
TARGET = -target x86_64-mingw64

SDKINC = ../../sdk/include
SYSTEM64 = ../../base/system64
OUTPUT = $(SYSTEM64)/csrs.exe

.PHONY: all
all:  $(OFILES)
	$(CC) $(TARGET) $(CFLAGS) $(OFILES) -o $(OUTPUT)

%.o: %.c
	$(CC) -c $(TARGET) $(CFLAGS) -I $(SDKINC) $< -o $@
