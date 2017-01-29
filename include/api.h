/**
 * Copyright (c) Project Iota
 *
 * WeIO_firmware is licensed under an Apache license, version 2.0 license.
 * All rights not explicitly granted in the Apache license, version 2.0 are reserved.
 * See the included LICENSE file for more details.
 *
 * @author      Paul Rathgeb <paul.rathgeb@skynet.be>
 */

#ifndef _API_H_
#define _API_H_

typedef enum {
    STATUS_OK,
    STATUS_KO,
    STATUS_NOT_IMPLEMENTED,
	STATUS_UNRECOGNIZED
} API_STATUS;

void SendStatus(API_STATUS status);
void RegisterFunctions(void);
void pinMode(int argc, JSMN_PARAMS_t argv);
void digitalWrite(int argc, JSMN_PARAMS_t argv);
void digitalRead(int argc, JSMN_PARAMS_t argv);

#endif
