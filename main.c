#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "periph/pwm.h"
#include "periph/gpio.h"
#include "usb_enum.h"
#include "IAP.h"
#include <stdio.h>
#include "erpc.h"

uint32_t GUID[4];
CMDIFStream stream;

void digitalWrite(int argc, JSMN_PARAMS_t argv)
{
    uint32_t p;
    p = strtol((char*)argv[0], NULL, 10);
    if ((p % 1000) == 0)
        gpio_toggle(GPIO_PIN(1, 14));
    char* str = "{ \"Status\": \"OK\" }";
    stream.write(str, strlen(str));
}



int main(void)
{
    IAP_GetSerialNumber(GUID);
       
    gpio_init(GPIO_PIN(1, 14), GPIO_OUT);
    
    CDC_Init(&stream, GUID);

    erpcAddFunction("digitalWrite", digitalWrite);

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
