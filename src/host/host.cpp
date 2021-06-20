// Rebellion
//
// File : host.cpp
// Author : (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020, 2021
// License : LGPLv3

#include <stdio.h>
#include <iostream>
#include <string>

#include <json.hpp>
using json = nlohmann::json;

#include <chrono>
#include <thread>

extern "C" {
    #include "rebellion.h"
}

int rpc_callback_func(rebellion_message_format mf, rebellion_message_type mt, const uint8_t *udata, uint32_t len) {

    if (mf == REBELLION_MF_JSON) {
        std::cout << "host main: rpc_callback_func called" <<  std::endl;
        const char *data = (const char *)udata;
        json jsonrpc = json::parse(data);

        std::cout << "json parsed: " <<  jsonrpc << std::endl;
        if (jsonrpc.contains("error"))
        {
            std::cerr << "Error: " << jsonrpc["error"] << std::endl;
        }
        if (jsonrpc.contains("event"))
        {
            std::cout << "Event: " << jsonrpc["event"] << std::endl;
            if (jsonrpc.contains("device")) {
                std::cout << "Device: " << jsonrpc["device"] << std::endl;
            }

            if (jsonrpc.contains("serial")) {
                std::cout << "Serial: " << jsonrpc["serial"] << std::endl;
            }

            if (jsonrpc["event"] == "rpc.callback.registered") {
                std::cout << "Callback function registered" << std::endl;
            } else {
                std::cout << "Data: " << jsonrpc["data"].dump(4) << std::endl;
                //std::this_thread::sleep_for(std::chrono::seconds(2));
                //std::this_thread::sleep_for(std::chrono::milliseconds(100));
                //exit(1);
            }
        }

    } else {
        std::cerr << "host main: message format " << rebellion_message_format_str(mf) << " not supported yet" << std::endl;
    }

    return 0;
}

int main()
{
    std::cout << "host main: start" << std::endl;

    std::cout << "host main: setting callback func" << std::endl;
    rebellion(rpc_callback_func);

    std::cout << "host main: calling rebellion rpc" << std::endl;
    json jsonrpc = {
        { "method", "rpc" },
        { "params", { 
            "foo", "bar" 
            } 
        },
        { "id", 1 }
    };
    std::string s = jsonrpc.dump();
    const uint8_t *udata = (uint8_t *)s.c_str();
    int res = rebellion_rpc(
                        rebellion_message_format::REBELLION_MF_JSON, 
                        rebellion_message_type::REBELLION_MT_REQ, 
                        udata, 
                        s.length());

    std::cout << "host main: calling rebellion loop" << std::endl;
    rebellion_loop(0);

    std::cout << "host main: finished" << std::endl;

    std::cout << "=====" << std::endl;

    return 0;
}
