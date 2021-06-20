// Rebellion
//
// File: niproto.cpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020,2021
// License: LGPLv3

#include "niproto.hpp"
#include "niproto_pimpl.hpp"

#include <memory>

NIIPC::NIIPC(const char *name)
    :   _name(name),
        _callback(nullptr),
        _niipc (new _NIIPC(this) ) {
            printf("Constructing NIIPC: %s\n", name);
}

NIIPC::~NIIPC() {
    printf("Deconstructing NIIPC\n");
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
