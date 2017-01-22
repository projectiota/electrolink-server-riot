APPLICATION = UPER-Release
RIOTBASE ?= $(CURDIR)/../RIOT
BOARD ?= weio
DIRS += tools/lpcrc/
PORT ?= /dev/tty.usbserial

#USEMODULE += xtimer
#USEMODULE += dht
#FEATURES_REQUIRED += periph_timer

INCLUDES += -I$(CURDIR)/include
QUIET ?= 0

#include the USB support
DIRS += usb
INCLUDES += -I$(CURDIR)/usb/include
USEMODULE += usb 


crc:
	./tools/lpcrc/lpcrc $(BINDIR)/$(APPLICATION).bin	

include $(RIOTBASE)/Makefile.include
