/*
 * Copyright (C) 2015 Kaspar Schleiser <kaspar@schleiser.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     xtimer_examples
 * @{
 *
 * @file
 * @brief       example application for setting a periodic wakeup
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 *
 * @}
 */

#include <stdio.h>
#include "periph/pwm.h"
#include "periph/gpio.h"
#include "usb_enum.h"
#include "IAP.h"

uint32_t GUID[4];
CMDIFStream stream;

int main(void)
{
    IAP_GetSerialNumber(GUID);
       
    gpio_init(256 + 15, GPIO_OUT);
    
    char buf[64];
    CDC_Init(&stream, GUID);
   
    uint32_t size;
    while(1) {
        size = stream.available();
        if (size) {
            stream.read((uint8_t*)&buf, size);
            printf("%s\n", (char*)&buf);
            gpio_toggle(256 + 15);
        }
    }

    return 0;
}
