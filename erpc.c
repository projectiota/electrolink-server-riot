/**
 * Copyright (c) Project Iota
 *
 * uCbor is licensed under an Apache license, version 2.0 license.
 * All rights not explicitly granted in the Apache license, version 2.0 are reserved.
 * See the included LICENSE file for more details.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jsmn.h"
#include "erpc.h"

#define JSONRPC_METHOD_KEY "method"
#define JSONRPC_PARAMS_KEY "params"
#define JSONRPC_REPLY_TO_KEY "rto"
#define JSONRPC_NULL "null"

/**
 * Globals
 */
static unsigned char method[8] = {0};
static unsigned char fncIdx = 0;
static unsigned char params[8][16] = {0};
static unsigned char replyTo[32] = {0};
static unsigned char paramNb = 0;

/**
 * fncTable is an array of function pointers.
 * Function pointers are installed by the user.
 */
#define FNC_TABLE_SIZE  1024
void (*fncTable[FNC_TABLE_SIZE])(int argc, char *argv[]) = {NULL};

/**
 * Fowler/Noll/Vo (FNV) hash function, variant 1a
 */
static size_t fnv1a_hash(const unsigned char* cp)
{
    size_t hash = 0x811c9dc5;
    while (*cp) {
        hash ^= (unsigned char) *cp++;
        hash *= 0x01000193;
    }
    return hash;
}

/**
 * Helper function to compare strings
 */
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
            strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

/**
 * Parse JSON and call adequate function from lookup table
 */
int erpcCall(char* req, uint8_t size)
{
    int i;
    int r;
    jsmn_parser p;
    jsmntok_t t[16]; /* We expect no more than 128 tokens */
    jsmn_init(&p);

    r = jsmn_parse(&p, req, size, t, sizeof(t)/sizeof(t[0]));
    if (r < 0) {
#ifdef ERPC_DEBUG
        printf("Failed to parse JSON: %d\n", r);
#endif
        return 1;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT) {
#ifdef ERPC_DEBUG
        printf("Object expected\n");
#endif
        return 1;
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {
        if (jsoneq(req, &t[i], JSONRPC_METHOD_KEY) == 0) {

            int size = t[i+1].end-t[i+1].start;
        
            /* We may use strndup() to fetch string value */
            memcpy(method, req + t[i+1].start, t[i+1].end-t[i+1].start);
            method[size] = '\0';
#ifdef ERPC_DEBUG
            printf("- Method: %s\n", method);
#endif

            fncIdx = fnv1a_hash(method) % FNC_TABLE_SIZE;

#ifdef ERPC_DEBUG
            printf("- Function Table Index: %d\n", fncIdx);
#endif
            i++;
        } else if (jsoneq(req, &t[i], JSONRPC_PARAMS_KEY) == 0) {
            int j;

#ifdef ERPC_DEBUG
            printf("- Params:\n");
#endif
            /** Reset paramNb */
            paramNb = 0;

            if (t[i+1].type != JSMN_ARRAY) {
                continue; /* We expect params to be an array of strings */
            }
            for (j = 0; j < t[i+1].size; j++) {
                jsmntok_t *g = &t[i+j+2];
                int paramSize = g->end - g->start;
                memcpy(params[paramNb], req + g->start, paramSize);
                params[paramNb][paramSize] = '\0';

#ifdef ERPC_DEBUG
                printf("  * %s\n", params[paramNb]);
#endif
                paramNb++;
            }
            i += t[i+1].size + 1;
        } else if (jsoneq(req, &t[i], JSONRPC_REPLY_TO_KEY) == 0) {
            int size = t[i+1].end-t[i+1].start;

            /* We may want to do strtol() here to get numeric value */
            memcpy(replyTo, req + t[i+1].start, size);
            replyTo[size] = '\0';
#ifdef ERPC_DEBUG
            printf("- replyTo: %s\n", replyTo);
#endif
            i++;
        } else {
#ifdef ERPC_DEBUG
            printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
            req + t[i].start);
#endif
        }
    }

    /** Call the function */
    fncTable[fncIdx](paramNb, (char **)params);
    return 0;
}

/**
 * Set function table:
 * user declares custom functions and stores them in the table
 * in his high level program, then tells erpc library to use these functions.
 *
 * Erpc is platform agnostic - it knows only to parse JSON and call
 * method with name `fncIdx` with correct parameters. What will this method
 * actually do, it is on user to define.
 */
void erpcAddFunction(char* fncName, void (*f)(int argc, char *argv[]))
{
    fncIdx = fnv1a_hash((const unsigned char *)fncName) % FNC_TABLE_SIZE;
    fncTable[fncIdx] = f;
}
