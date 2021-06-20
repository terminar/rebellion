// Rebellion
//
// File: rebellion.h
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020,2021
// License: LGPLv3

#ifndef _REBELLION_H_
#define _REBELLION_H_

#include <inttypes.h>

    #ifdef _WIN32

        #ifdef REBELLION_LIB
            #define REBELLION_DECL __declspec(dllexport)
        #else
            #define REBELLION_DECL __declspec(dllimport)
        #endif

    #else

        #ifdef REBELLION_LIB
            #define REBELLION_DECL __attribute__((visibility("default")))
        #else
            #define REBELLION_DECL
        #endif

    #endif

typedef enum
{
    REBELLION_MT_REQ, //request
    REBELLION_MT_RES, //result
    REBELLION_MT_EV,  //event
    REBELLION_MT      //count/end of message types
} rebellion_message_type;

typedef enum
{
    REBELLION_MF_JSON,
    REBELLION_MF //count/end of message formats
} rebellion_message_format;

REBELLION_DECL const char *rebellion_message_type_str(rebellion_message_type mt);
REBELLION_DECL const char *rebellion_message_format_str(rebellion_message_format mt);

//rebellion callback
typedef int (*rebellion_rpc_callback)(
                    rebellion_message_format mf, 
                    rebellion_message_type mt, 
                    const uint8_t *data, 
                    uint32_t len);

REBELLION_DECL int rebellion(rebellion_rpc_callback cb);
REBELLION_DECL int rebellion_rpc(
                    rebellion_message_format mf, 
                    rebellion_message_type mt, 
                    const uint8_t *udata, 
                    uint32_t len);

REBELLION_DECL int rebellion_loop(uint32_t looptime);

#endif
