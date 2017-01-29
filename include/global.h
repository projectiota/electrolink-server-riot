/**
 * Copyright (c) Project Iota
 *
 * WeIO_firmware is licensed under an Apache license, version 2.0 license.
 * All rights not explicitly granted in the Apache license, version 2.0 are reserved.
 * See the included LICENSE file for more details.
 *
 * @author      Paul Rathgeb <paul.rathgeb@skynet.be>
 */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "erpc.h"
#include "api.h"

typedef struct CMDStream {
	uint32_t (*available)(void);					/**< Function that returns the ammount of available data */
	uint32_t (*read)(uint8_t *buf, uint32_t len);	/**< Function that reads the data into the buffer */
	uint8_t  (*readByte)(void);
	void 	 (*write)(char *buf, uint32_t len);	/**< Function that write the data into the stream */
} CMDStream;

extern CMDStream stream;

#endif
