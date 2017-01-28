APPLICATION = WeIO_firmware
RIOTBASE ?= $(CURDIR)/../RIOT
BOARD ?= weio
DIRS += tools/lpcrc/
PORT ?= /dev/tty.usbserial
CFLAGS += -DLOG_LEVEL=LOG_ALL
LINKFLAGS += -Wl,--print-memory-usage

USEPKG += jsmn
#USEMODULE += xtimer
#USEMODULE += dht
#FEATURES_REQUIRED += periph_timer

INCLUDES += -I$(CURDIR)/include
QUIET ?= 1
#CFLAGS += -DDEVELHELP

#include the USB support
DIRS += usb
INCLUDES += -I$(CURDIR)/usb/include
USEMODULE += usb 


crc:
	./tools/lpcrc/lpcrc $(BINDIR)/$(APPLICATION).bin	

include $(RIOTBASE)/Makefile.include
