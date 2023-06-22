// Rebellion
//
// File: main.hpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <string>
#include <map>

#include "platform.hpp"

extern "C"
{
    #include "rebellion.h"
    //    #include "lua.h"
}

#ifdef _WIN32
    #include <windows.h>
    #include <SetupAPI.h>
    #include <initguid.h>
    #include <Usbiodef.h>

    //#define PATH_SCRIPTS "c:\\src\\reakontrol\\srcnative\\scripts\\"
    //#define PATH_LOG "c:\\src\\reakontrol\\srcnative\\print.log"

    #include <direct.h>

#else
    #include <sys/types.h>
    #include <sys/time.h>

    #include <sys/stat.h>
    #include <errno.h>

    #include <unistd.h> //getcwd()

    //#define PATH_SCRIPTS "/Users/terminar/src/NI/reaKontrol/srcnative/scripts/"
    //#define PATH_LOG "/Users/terminar/src/NI/reaKontrol/srcnative/print.log"

    typedef unsigned int DWORD;

#endif

#define PATH_SCRIPTS "scripts/"
#define PATH_LOG "print.log"

//const char *getAbsRootPath(const char *name);

#define PATH_LUASCRIPT_START PATH_SCRIPTS "start.lua"
#define PATH_LUASCRIPT_END PATH_SCRIPTS "end.lua"
#define PATH_LUASCRIPT_PROC PATH_SCRIPTS "proc.lua"

extern bool running;
extern FILE *debfp;

extern rebellion_rpc_callback _rebellion_rpc_callback;

enum DEB_LEVEL
{
    DEB_ERROR,
    DEB_WARN,
    DEB_NOTE,
    DEB_INFO,
    DEB_DEBUG
};

extern DEB_LEVEL deb_level;

//#define DEB(x) MessageBoxA(NULL,x,"Reaper",MB_OK)
#define DEB(x) printf("%s\n",x); debfp = fopen(PATH_LOG,"w"); if (debfp) { fprintf(debfp,"%s\n", x); fclose(debfp);}
#define DEBf(...) printf(__VA_ARGS__); debfp = fopen(PATH_LOG,"a"); if (debfp) { fprintf(debfp,__VA_ARGS__); fclose(debfp);}

#define DEBL(level,x) if (level <= deb_level) { printf("%s\n",x); debfp = fopen(PATH_LOG,"w"); if (debfp) { fprintf(debfp,"%s\n", x); fclose(debfp);} }
#define DEBLf(level,...) if (level <= deb_level) { printf(__VA_ARGS__); debfp = fopen(PATH_LOG,"a"); if (debfp) { fprintf(debfp,__VA_ARGS__); fclose(debfp);} }

#endif
