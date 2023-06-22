// Rebellion
//
// File: main.cpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#include <lua.hpp>
#include <lauxlib.h>

#include <string>
#include <cstring>
#include <vector>
#include <memory>

#include <chrono>
#include <thread>

using namespace std;

#include "main.hpp"

#include <json.hpp>
using json = nlohmann::json;

extern "C" {
    #include "rebellion.h"
}

#include "luafuncs.hpp"

#include "foo.hpp"
#include "niproto.hpp"


#ifdef _WIN32

#else

DWORD GetTickCount()
{
    // could switch to mach_getabsolutetime() maybe
    struct timeval tm = {
        0,
    };
    gettimeofday(&tm, NULL);
    return (DWORD)(tm.tv_sec * 1000 + tm.tv_usec / 1000);
}

#endif //_WIN32

DEB_LEVEL deb_level = DEB_WARN; //DEB_DEBUG
FILE *debfp;
bool running = true;

//REBELLION LIB GLOBALS
rebellion_rpc_callback _rebellion_rpc_callback;

extern "C"
{
    REBELLION_DECL const char *rebellion_message_type_str(rebellion_message_type mt) {

        const char *rmt[] = {
            "REBELLION_MT_REQ",
            "REBELLION_MT_RES",
            "REBELLION_MT_EV",
            "REBELLION_MT"
        };

        if (mt > REBELLION_MT) {
            return "";
        }
        return rmt[mt];
    }

    REBELLION_DECL const char *rebellion_message_format_str(rebellion_message_format mf) {

        const char *rmf[] = {
            "REBELLION_MF_JSON",
            "REBELLION_MF"
        };
        if (mf > REBELLION_MF) {
            return "";
        }
        return rmf[mf];
    }

    REBELLION_DECL int rebellion_loop(uint32_t looptime) {
        int32_t llooptime = looptime;
        uint32_t waitmax = (llooptime == 0 ? 100 : llooptime);

        while (llooptime >= 0) //if 0, it's an endless loop. if > 0 it will only run once
        {
            auto start = std::chrono::steady_clock::now();
            //DEBf("C> main: luaRun(%i)\n",i);
            DEBLf(DEB_DEBUG, "C> REBELLION LIB: rebellion_loop run\n");
            luaRun();

            //Throttling
            auto end = std::chrono::steady_clock::now();
            auto difTime = end - start;
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(difTime).count();
            DEBLf(DEB_DEBUG, "C> REBELLION LIB: loop runtime: %lld\n", diff);
            if (diff < waitmax)
            {
                DEBLf(DEB_DEBUG, "C> REBELLION LIB: loop throttling\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(waitmax - diff));
            }
            if (llooptime > 0) {
                DEBLf(DEB_DEBUG, "C> REBELLION LIB: rebellion_loop end\n");
                llooptime = -1;
            }
        }
        DEBLf(DEB_DEBUG, "C> REBELLION LIB: leaving rebellion_loop\n");
        return 0;
    }

    REBELLION_DECL int rebellion_rpc(
                            rebellion_message_format mf, 
                            rebellion_message_type mt, 
                            const uint8_t *udata, 
                            uint32_t len)
    {
        DEBLf(DEB_DEBUG, "C> REBELLION LIB: rpc called\n");
        if (udata != NULL && len > 0)
        {
            return luaRpc(mf, mt, udata, len);
        }
        return 0;
    }

    REBELLION_DECL int rebellion(rebellion_rpc_callback cb)
    {
        if (cb != NULL) { //start
            DEBLf(DEB_INFO, "C> REBELLION LIB: rebellion START, starting lua\n");
            DEBLf(DEB_INFO, "C> REBELLION LIB: luaStart()\n");
            luaStart();

            DEBLf(DEB_INFO, "C> REBELLION LIB: registering callback !\n ");
            _rebellion_rpc_callback = cb;

            json jsonrpc = {
                {"event", "rpc.callback.registered"}
            };
            std::string s = jsonrpc.dump();
            DEBLf(DEB_DEBUG, "C> REBELLION LIB: rebellion register_cb\n => testing callback, sending '%s'\n", s.c_str());

            uint8_t *udata = (uint8_t *)s.c_str();
            _rebellion_rpc_callback(REBELLION_MF_JSON, REBELLION_MT_EV, udata, s.length());
        } else { //end
            DEBLf(DEB_INFO, "C> REBELLION LIB: rebellion END, stopping lua\n");
            DEBLf(DEB_INFO, "C> main: luaEnd()\n");
            luaEnd();

            DEBLf(DEB_INFO, "C> REBELLION LIB: unregistering callback !\n ");
            _rebellion_rpc_callback = NULL;
        }
        return 1;
    }
}
