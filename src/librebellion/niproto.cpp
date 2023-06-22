// Rebellion
//
// File: niproto.cpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#include "niproto.hpp"
#include "niproto_pimpl.hpp"

#include <memory>


NIIPC::NIIPC(const char *name)
    :   _name(name),
        _callback(nullptr),
        _niipc (new _NIIPC(this) ) {
            printf("Constructing NIIPC: %s\n", name);
            pthread_mutex_init(&_cqlock, NULL);

}

NIIPC::~NIIPC() {
    printf("C> Deconstructing NIIPC\n");
    if (this->hasCallback()) {
        this->setCallback(nullptr);
    }
    delete _niipc;
}

std::string NIIPC::getName() {
    return _name;
}

bool NIIPC::open() {
    return _niipc->open();
}

bool NIIPC::create() {
    return _niipc->create();
}

bool NIIPC::loop(double seconds) {
    bool res;
    do {
        res = this->fireCallbackIfResultQueued();
        if (res) printf("C> fired new Callback\n");
    } while (res);
    return _niipc->loop(seconds);
}

bool NIIPC::close() {
    return _niipc->close();
}

std::unique_ptr<NIIPC::Data> NIIPC::send(std::unique_ptr<NIIPC::Data> data) {
	return _niipc->send(std::move(data));
}

bool NIIPC::push(std::unique_ptr<NIIPC::Data> data) {
	return _niipc->push(std::move(data));
}

void NIIPC::setCallback(NIIPC::Callback callback) {
    _callback = callback;
}

bool NIIPC::hasCallback() { 
    return (this->_callback != nullptr ? true : false); 
}

void NIIPC::queueCallbackResult(std::unique_ptr<NIIPC::Data> data) {
    printf("C> %s: trying to queue callback data\n", this->_name.c_str());
	if (this->hasCallback()) {
        printf("C> %s: has callback, caching callback result\n", this->_name.c_str());
        pthread_mutex_lock(&_cqlock);
        this->_callbackResultQueue.push(std::move(data));
        pthread_mutex_unlock(&_cqlock);
    } else {
        printf("%s: Can't cache callback result, not set\n", this->_name.c_str());
    }
}
/*
std::unique_ptr<NIIPC::Data> NIIPC::fireCallback(std::unique_ptr<NIIPC::Data> data) {
    printf("%s: trying to fire callback\n", this->_name.c_str());
	if (this->hasCallback()) {
        printf("%s: has callback, executing _callback\n", this->_name.c_str());
        return this->_callback(std::move(data));
        printf("%s: back from _callback(std::move data)\n", this->_name.c_str());
    } else {
        printf("%s: Can't fire callback, not set\n", this->_name.c_str());
    }
    return NULL;
}
*/

bool NIIPC::fireCallbackIfResultQueued() {
    if (!this->_callbackResultQueue.empty()) {
        pthread_mutex_lock(&_cqlock);

        printf("C> %s: trying to fire callback (%i remaining)\n", this->_name.c_str(), this->_callbackResultQueue.size());
        if (this->hasCallback()) {
            printf("C> %s: has callback, executing _callback\n", this->_name.c_str());
            this->_callback(std::move(this->_callbackResultQueue.front()));
            printf("C> %s: back from _callback(std::move data)\n", this->_name.c_str());
        } else {
            printf("C> %s: Can't fire callback, not set\n", this->_name.c_str());
        }
        this->_callbackResultQueue.pop();

        pthread_mutex_unlock(&_cqlock);

        return true;
    }

    return false;
}
