ifndef PSL1GHT
$(error Set PSL1GHT to the pinned SDK root)
endif
TITLE := PS3 Trophy Read-Only Probe
APPID := TROPH2026
CONTENTID := UP0001-$(APPID)_00-0000000000000001
TARGET := trophy-probe
BUILDDIR := $(CURDIR)/build
include $(PSL1GHT)/ppu_rules

.PHONY: all
all: $(TARGET).pkg

$(TARGET).elf: source/probe.c
	$(CC) -std=c99 -O2 -Wall -Wextra -Werror $(MACHDEP) $(LIBPSL1GHT_INC) $< $(LIBPSL1GHT_LIB) -lsysutil -llv2 -o $@

$(TARGET).self: $(TARGET).elf
$(TARGET).pkg: $(TARGET).self
