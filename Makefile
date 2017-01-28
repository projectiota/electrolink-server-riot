# Application name
APPLICATION = WeIO_firmware
# PATH to RIOT
RIOTBASE ?= $(CURDIR)/../RIOT
# Target board
BOARD ?= weio

# UART port, used for 'make term'
PORT ?= /dev/tty.usbserial

# LOG verbosity (LOG_ALL, LOG_ERROR, LOG_INFO, LOG_NONE)
CFLAGS += -DLOG_LEVEL=LOG_NONE

# Build silently
QUIET ?= 1

# Use JSMN from RIOT package
USEPKG += jsmn

INCLUDES += -I$(CURDIR)/include


ifeq ($(BOARD), weio)
DIRS += tools/lpcrc/
DIRS += usb/lpc11u34
USEMODULE += lpc11u34
INCLUDES += -I$(CURDIR)/usb/lpc11u34/include
CFLAGS += -DUSE_USB
CFLAGS += -DUSB_LPC11U34
LINKFLAGS += -Wl,--print-memory-usage
endif

crc:
	./tools/lpcrc/lpcrc $(BINDIR)/$(APPLICATION).bin	

include $(RIOTBASE)/Makefile.include
