/**
 * Copyright (c) Project Iota
 *
 * WeIO_firmware is licensed under an Apache license, version 2.0 license.
 * All rights not explicitly granted in the Apache license, version 2.0 are reserved.
 * See the included LICENSE file for more details.
 *
 * @author      Paul Rathgeb <paul.rathgeb@skynet.be>
 */

#include "global.h"
#include "periph/gpio.h"

#ifdef USE_USB
#       include "usb_enum.h"
#       include "IAP.h"
        uint32_t GUID[4];
#endif

int main(void)
{
#ifdef USE_USB
    IAP_GetSerialNumber(GUID);
    CDC_Init(GUID);
    USBRegisterStream(&stream);
#endif

    RegisterFunctions();

    uint8_t size = 0;
    char buf[64];
    while(1) {
        size = stream.available();
        if (size) {
            stream.read((uint8_t*)&buf, size);
            erpcCall((char*)&buf, size);
        }
    }

    return 0;
}
