#include "global.h"
#include "periph/gpio.h"

void digitalWrite(int argc, JSMN_PARAMS_t argv)
{
    uint32_t p;
    p = strtol((char*)argv[0], NULL, 10);
    if ((p % 250) == 0)
        gpio_toggle(GPIO_PIN(1, 14));
    char* str = "{ \"Status\": \"OK\" }";
    stream.write(str, strlen(str));
}
