#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "periph/pwm.h"
#include "periph/gpio.h"
#include "usb_enum.h"
#include "IAP.h"
#include <stdio.h>
#include "erpc.h"


void digitalWrite(int argc, JSMN_PARAMS_t argv)
{
    gpio_toggle(GPIO_PIN(1, 13));
    //JSMN_PARAMS_t params = {{0}};
    //memcpy(params, argv, sizeof(JSMN_PARAMS_t));

    uint32_t p;
    uint8_t i = 0;
    for (i = 0; i < argc; i++) {
        p = strtol((char*)argv[i], NULL, 10);
        printf("%lu\n", p);
    }
}


uint32_t GUID[4];
CMDIFStream stream;

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
