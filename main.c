#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "periph/pwm.h"
#include "periph/gpio.h"
#include "usb_enum.h"
#include "IAP.h"
#include <stdio.h>
#include "erpc.h"

void digitalWrite(int argc, char *argv[])
{
    gpio_toggle(GPIO_PIN(1, 13));
}

void digitalRead(int argc, char *argv[])
{
    printf("FNC: digitalRead()\n");
}

uint32_t GUID[4];
CMDIFStream stream;

int main(void)
{
    IAP_GetSerialNumber(GUID);
       
    gpio_init(GPIO_PIN(1, 14), GPIO_OUT);
    
    CDC_Init(&stream, GUID);

    erpcAddFunction("digitalWrite", digitalWrite);
    erpcAddFunction("digitalRead", digitalRead);
    volatile uint8_t size;
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
