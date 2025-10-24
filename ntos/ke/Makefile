#
# ALGAE Kernel Build Configuration
# Copyright (c) 2025, Ian Moffett and the Algae team.
# All rights reserved.
# Provided under the BSD 3-Clause license.
#

include ../data/sys.mk

SDKDIR =

CFILES = $(shell find . -name "*.c")
OBJECTS = $(CFILES:.c=.o)
DEPS = $(CFILES:.c=.d)

CFLAGS = -target x86_64-elf $(MD_CFLAGS) -MMD -I$(SDKDIR)/include/
CC =
LD =

.PHONY: all
all: $(OBJECTS)

-include $(DEPS)
%.c: %.o
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS)
