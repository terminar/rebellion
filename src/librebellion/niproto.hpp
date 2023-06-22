// Rebellion
//
// File: niproto.hpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#ifndef _NIPROTO_H_
#define _NIPROTO_H_

//stringify a defined text
#define _MACRO_STR(s) #s
#define MACRO_STR(s) _MACRO_STR(s)

#include <stdint.h>
#include <cstddef>
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <functional>

#include <pthread.h>

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

class NIIPC {
    public:
        typedef std::vector<uint8_t> Data;

        //typedef void CallbackData;
        typedef std::function<std::unique_ptr<NIIPC::Data> (std::unique_ptr<NIIPC::Data> data)> Callback;

        NIIPC(const char *name);
        ~NIIPC();
        
        bool open();
        
        bool create();
        bool loop(double seconds);
        
        bool close();

        std::unique_ptr<NIIPC::Data> send(std::unique_ptr<NIIPC::Data> data);
        bool push(std::unique_ptr<NIIPC::Data> data);
    
        std::string getName();
        void setCallback(Callback callback);
        bool hasCallback();
        void queueCallbackResult(std::unique_ptr<NIIPC::Data> data);
        //std::unique_ptr<NIIPC::Data> fireCallback(std::unique_ptr<NIIPC::Data> data);
        bool fireCallbackIfResultQueued();

    private:
        std::string _name;
        std::queue< std::unique_ptr<NIIPC::Data> > _callbackResultQueue;
        Callback _callback;
        pthread_mutex_t _cqlock;

        //forward declaration for pimpl
        class _NIIPC;
        _NIIPC *_niipc;
};

#endif
