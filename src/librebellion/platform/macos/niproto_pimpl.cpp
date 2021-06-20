// Rebellion
//
// File: niproto_pimpl.cpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020,2021
// License: LGPLv3

#include "niproto_pimpl.hpp"
#include "../../niproto.hpp"

#include <iostream>
#include <platform.hpp>

NIIPC::_NIIPC::_NIIPC(NIIPC *parent) 
    : _parent(parent)
    , _portRef(NULL) {
    std::cout
        << MACRO_STR(PLATFORM_NAME) 
        << "> Name: " 
        << this->_parent->getName().c_str() 
        << std::endl;
}

NIIPC::_NIIPC::~_NIIPC() {
    
}

bool NIIPC::_NIIPC::open() {

    //open port existing
    if (this->_portRef) {
        return false;
    }

    CFStringRef cfName = CFStringCreateWithCString(kCFAllocatorDefault,
                                                   this->_parent->getName().c_str(),
                                                   kCFStringEncodingASCII);
    this->_portRef = CFMessagePortCreateRemote(kCFAllocatorDefault,
                                     cfName);

    return (this->_portRef ? true : false);
}

bool NIIPC::_NIIPC::create() {
    printf("Create called\n");
    //open port existing
    if (this->_portRef) {
        printf("port is set!\n");
        return false;
    }

    CFMessagePortCallBack callback = (CFMessagePortCallBack) _port_callback;
    //TODO: do init stuff with callback

    Boolean              shouldFreeInfo;
    CFStringRef          cfName;
    CFMessagePortRef     port;
    CFMessagePortContext ctx;

    cfName = CFStringCreateWithCString(kCFAllocatorDefault,
                                       _parent->getName().c_str(),
                                       kCFStringEncodingASCII);

    ctx.version = 0;
    ctx.info = this->_parent;
    ctx.retain = NULL;//CFRetain;
    ctx.release = NULL;//CFRelease;
    ctx.copyDescription = NULL;

    printf("Trying to create local listening port: [%s]\n", _parent->getName().c_str() );
    this->_portRef = CFMessagePortCreateLocal(kCFAllocatorDefault,
                                    cfName,
                                    callback,
                                    &ctx,
                                    &shouldFreeInfo);

    //TODO: ?!? check! not used currently?
    /*
    https://developer.apple.com/documentation/corefoundation/1543289-cfmessageportcreatelocal?language=objc

    shouldFreeInfo

        A flag set by the function to indicate whether the info member of context 
        should be freed. The flag is set to true on failure or if a local port 
        named name already exists, false otherwise. shouldFreeInfo can be NULL. 
    */
    if (shouldFreeInfo) {
        printf("shouldFreeInfo is true but we don't want to free ourself!\n");
//        free(info);
    }

    if (this->_portRef) {
        //THIS IS NEEDED!
        CFMessagePortSetDispatchQueue(this->_portRef,
                                    dispatch_get_main_queue());

        printf("ok, created port\n");
    }

    return (this->_portRef ? true : false);
}

void
_callCFRunLoop(double seconds) {
    //printf("callCFRunLoop()\n");

    //this should start the message queue for the port, otherwise the data will hang
    //maybe use this? https://stackoverflow.com/questions/12825620/cfmessageport-weirdness
    //mode, seconds, returnAfterSourceHandled
    CFRunLoopRunResult runLoopRunReturnValue = CFRunLoopRunInMode(kCFRunLoopDefaultMode, seconds, true);

    if (runLoopRunReturnValue == kCFRunLoopRunHandledSource) {
        printf("handled source\n");
        //return returned;
    } else {
        //TODO: check if this is an exception
        //printf("Exception?\n");
        // Throw exception or whatever
        // (although this will never be called using the above implementation
        // since [NSDate distantFuture] is wayy into the future...)
    }
}

bool NIIPC::_NIIPC::loop(double seconds) {
    _callCFRunLoop(seconds);
   return false;
}

bool NIIPC::_NIIPC::close() {
    return false;
}

std::unique_ptr<NIIPC::Data> NIIPC::_NIIPC::send(std::unique_ptr<NIIPC::Data> data) {

    //add debug error message: no open port
    if (!this->_portRef || !data) {
        return nullptr;
    }

    CFDataRef _dataRef = sendMsg(this->_portRef, data->data(), data->size());
    if (_dataRef) {
        const uint8_t *ptr = static_cast<const uint8_t*>(CFDataGetBytePtr(_dataRef));
        auto result = std::make_unique<NIIPC::Data>(ptr,ptr+CFDataGetLength(_dataRef));
        CFRelease(_dataRef);
        return result;
    }

    return nullptr;
}

bool NIIPC::_NIIPC::push(std::unique_ptr<NIIPC::Data> data) {

    //on macOS there is no one-way push like "write" on windows
    auto result = this->send(std::move(data));
    return true;
}
