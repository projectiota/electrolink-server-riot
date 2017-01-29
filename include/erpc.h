/**
 * Copyright (c) Project Iota
 *
 * Weio_firmware is licensed under an Apache license, version 2.0 license.
 * All rights not explicitly granted in the Apache license, version 2.0 are reserved.
 * See the included LICENSE file for more details.
 *
 * @author      Drasko Draskovic
 * @author      Paul Rathgeb <paul.rathgeb@skynet.be>
 */

#ifndef ERPC_H
#define ERPC_H

#define FNC_TABLE_SIZE 256

typedef unsigned char JSMN_PARAMS_t[8][16];
int erpcCall(char* req, uint8_t size);
void erpcAddFunction(char* fncName, void (*f)(int argc, JSMN_PARAMS_t argv));

#endif /** ERPC_H */
