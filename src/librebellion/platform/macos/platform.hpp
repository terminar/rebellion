// Rebellion
//
// File: platform.hpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#ifndef _OSX_H_
#define _OSX_H_

#include <CoreFoundation/CoreFoundation.h>


void dumpCFData(char *prefix, CFDataRef data);

CFDataRef sendMsg(CFMessagePortRef port, uint8_t *msg, size_t size);
CFDataRef _port_callback(CFMessagePortRef local, SInt32 msgid, 
                        CFDataRef data, void *info);

#endif
