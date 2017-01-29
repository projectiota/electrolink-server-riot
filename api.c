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

#define API_DEBUG

typedef enum {
    STATUS_OK,
    STATUS_KO,
    STATUS_NOT_IMPLEMENTED
} API_STATUS;

/* This is a private function to send back the command status */
void SendStatus(API_STATUS status)
{
    char* str = NULL;
    switch(status) {
        case STATUS_OK:
            str = "{ \"Status\": \"OK\" }";
            break;
        case STATUS_KO:
            str = "{ \"Status\": \"KO\" }";
            break;
        case STATUS_NOT_IMPLEMENTED:
            str = "{ \"Status\": \"Not implemented\" }";
            break;
    }
    stream.write(str, strlen(str));
}

void RegisterFunctions(void) {
    erpcAddFunction("pinMode", pinMode);
    erpcAddFunction("digitalWrite", digitalWrite);
    erpcAddFunction("digitalRead", digitalRead);
}

/**
 * Implementation of the Electrolink protocol, described here :
 * https://github.com/projectiota/electrolink/blob/master/electrolink-protocol.md
 */

void pinMode(int argc, JSMN_PARAMS_t argv)
{
    if(argc != 2) {
        SendStatus(STATUS_KO);
        return;
    }
    uint8_t pin, port, mode = 0;
    int error = 0;
    pin = atoi((char*)argv[0]);
    mode = atoi((char*)argv[1]);
    if (pin >= 32) {
        port = 1;
        pin -= 32;
    }
    error = gpio_init(GPIO_PIN(port, pin), mode);
#ifdef API_DEBUG
    printf("pinMode : port %d, pin %d, mode %d\n", port, pin, mode);
#endif
    if (!error) {
          SendStatus(STATUS_OK);
    }
    else {
          SendStatus(STATUS_KO);
    }
}

void digitalWrite(int argc, JSMN_PARAMS_t argv)
{
    if(argc != 2) {
        SendStatus(STATUS_KO);
        return;
    }
    uint8_t pin, port, value = 0;

    pin = atoi((char*)argv[0]);
    value = atoi((char*)argv[1]);
    if (pin >= 32) {
        port = 1;
        pin -= 32;
    }
#ifdef API_DEBUG
    printf("DigitalWrite : port %d, pin %d, value %d\n", port, pin, value);
#endif
    gpio_write(GPIO_PIN(port, pin), value);
    SendStatus(STATUS_OK);
}

void digitalRead(int argc, JSMN_PARAMS_t argv)
{
    if(argc != 1) {
        SendStatus(STATUS_KO);
        return;
    }
    uint8_t pin, port = 0;

    pin = atoi((char*)argv[0]);
    if (pin >= 32) {
        port = 1;
        pin -= 32;
    }
#ifdef API_DEBUG
    printf("digitalRead : port %d, pin %d\n", port, pin);
#endif
    uint8_t value = gpio_read(GPIO_PIN(port, pin));
    char str  [64];
    sprintf((char*) str, "{ \"digitalRead\": \"%s\", \"value\": \"%d\" }",(char*)argv[0],value);
    stream.write(str, strlen(str));
    printf("pinValue : %d\n", value);
}
