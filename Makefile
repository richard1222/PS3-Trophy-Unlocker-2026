ifndef PSL1GHT
$(error Set PSL1GHT to the pinned SDK root)
endif
TITLE := PS3 Trophy Discovery
APPID := TROPH2026
CONTENTID := UP0001-$(APPID)_00-0000000000000001
TARGET := trophy-startup
BUILDDIR := $(CURDIR)/build
include $(PSL1GHT)/ppu_rules

.PHONY: all
all: $(TARGET).pkg

$(TARGET).elf: source/startup.c source/probe.c source/rsxutil.c source/rsxutil.h
	$(CC) -std=gnu99 -O2 -Wall -Wextra -Werror -mcpu=cell $(MACHDEP) $(LIBPSL1GHT_INC) source/startup.c source/probe.c source/rsxutil.c $(LIBPSL1GHT_LIB) -lrsx -lgcm_sys -lsysutil -lrt -llv2 -lm -Wl,-Map,trophy-startup.map -o $@

$(TARGET).self: $(TARGET).elf
$(TARGET).pkg: $(TARGET).self

