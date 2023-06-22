// Rebellion
//
// File: platform.cpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#include "niproto_pimpl.hpp"
#include "platform.hpp"

#include <arpa/inet.h>

void dumpCFData(const char *prefix, CFDataRef data) {
        const unsigned char *buf = CFDataGetBytePtr(data);
        printf("\t\t--- %s --------------- %li\n\t\t", prefix, CFDataGetLength(data));
        for (int i=0; i < CFDataGetLength(data); i++) {
            printf("0x%02x[%c] ",buf[i], (buf[i] > 31 ? buf[i] : ' ') );
            if (i > 0 && (i+1) % 4 == 0) {
                printf("\t");
            }
            if (i > 0 && (i+1) % 8 == 0) {
                printf("\n\t\t");
            }
        }
        printf("\n\t\t=============================\n\n");
}

/*
    https://developer.apple.com/documentation/corefoundation/cfmessageportcallback

    Return Value

    Data to send back to the sender of the message. The system releases the 
    returned CFData object. Return NULL if you want an empty reply returned to 
    the sender. 
*/
CFDataRef
_port_callback(CFMessagePortRef local,
                          SInt32 msgid,
                          CFDataRef data,
                          void *info)
{
    printf("C> platform::_port_callback called >>>>>>>>>>>>>>\n");

    if (data && info) {
        dumpCFData("_port_callback:", data);
        uint8_t *response = (uint8_t*)CFDataGetBytePtr(data);
        CFIndex datalen = CFDataGetLength(data);

        auto vdata = std::make_unique<NIIPC::Data>(response, response + datalen);

        NIIPC *niipc = static_cast<NIIPC*>(info);
        if (niipc) {
            printf("C> platform::_port_callback: calling queueCallbackData\n");
    		niipc->queueCallbackResult(std::move(vdata));
            /*
            auto result = niipc->fireCallback(std::move(vdata));
            if (result != nullptr) {
                //printf("TODO: send return result from fireCallback - really needed???\n");
                //printf("C> platform::_port_callback: got return result, trying to sent it back via return\n");
                //OK, something was returned - return this to sender
                //CFDataRef res = CFDataCreate(kCFAllocatorDefault, (uint8_t*) result->data(), result->size());
                //return res;
            }
            */
        }
    }

    printf("C> platform::_port_callback: Done. Returning NULL <<<<<<<<<<<<<<\n");
    return NULL;
}

CFDataRef 
sendMsg(CFMessagePortRef port, uint8_t *msg, size_t size)
{
    CFDataRef msgData, returnData;
        
    msgData = CFDataCreate(kCFAllocatorDefault,
                           msg,
                           size);

    //dumpCFData("sendMsg >>>>>:", msgData);
                           
    if (!msgData) {
        printf("C> platform::sendMsg: Couldn't create message data\n");
        return NULL;
    }

    if (!port || !CFMessagePortIsValid(port)) {
        printf("C> platform::sendMsg: Either null or invalid message port\n");
        return NULL;
    }

    CFMessagePortSendRequest(port,
                             0,
                             msgData,
                             1000,
                             1000,
                             kCFRunLoopDefaultMode,
                             &returnData);
/*
    if (returnData != NULL) {
        dumpCFData("sendMsg <<<<<:", returnData);
    }
*/
    CFRelease(msgData);

    return returnData;
}
