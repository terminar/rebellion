// Rebellion
//
// File: niproto_pimpl.cpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#include "niproto_pimpl.hpp"
#include "../../niproto.hpp"

#include <iostream>
#include <platform.hpp>
/*
#include <pthread.h>
static pthread_mutex_t pclock;
static bool pclockinit = false;
*/

NIIPC::_NIIPC::_NIIPC(NIIPC *parent) 
    : _parent(parent)
    , _portRef(NULL) {

    std::cout
        << MACRO_STR(PLATFORM_NAME) 
        << "> Name: " 
        << this->_parent->getName().c_str() 
        << std::endl;
/*
    if (!pclockinit) {
        pthread_mutex_init(&pclock, NULL);
        pclockinit = true;
    }
*/
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

/*
#include <CoreFoundation/CoreFoundation.h>

#define kPortName "com.example.myport"

void MyPortCallBack(CFMessagePortRef port, SInt32 messageID, CFDataRef data, void *info)
{
    printf("Received a message on port %s with ID %d\n", kPortName, (int)messageID);
    // Handle the message data as needed
}

int main(int argc, char **argv)
{
    CFMessagePortRef myPort = CFMessagePortCreateLocal(NULL, CFSTR(kPortName), MyPortCallBack, NULL, NULL);
    if (!myPort) {
        fprintf(stderr, "Failed to create local port %s\n", kPortName);
        return -1;
    }
    
    // Send a message to another port
    CFMessagePortRef remotePort = CFMessagePortCreateRemote(NULL, CFSTR("com.example.remoteport"));
    if (!remotePort) {
        fprintf(stderr, "Failed to create remote port\n");
        return -1;
    }
    
    char message[] = "Hello, world!";
    CFDataRef messageData = CFDataCreate(NULL, (UInt8 *)message, sizeof(message));
    SInt32 messageID = 1;
    CFMessagePortSendRequest(remotePort, messageID, messageData, 1.0, 1.0, NULL, NULL);
    
    // Clean up
    CFMessagePortInvalidate(myPort);
    CFMessagePortInvalidate(remotePort);
    CFRelease(myPort);
    CFRelease(remotePort);
    CFRelease(messageData);
    
    return 0;
}

*/

/*
//----------------------------------
#include <mach/mach.h>

void my_callback(mach_msg_header_t *msg) {
    // Verwenden Sie keine lokalen Variablen 
    // Vermeiden Sie komplexe Operationen 
    // Verwenden Sie Mutexe, um den Zugriff auf gemeinsam genutzte Ressourcen zu synchronisieren 
    // Verwenden Sie Fehlerbehandlung, um sicherzustellen, dass der Callback ordnungsgemäß ausgeführt wird 
    // Debugging-Tools von macOS verwenden, um Probleme zu finden

}

int main() {
    mach_port_t port = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, 0);
    mach_msg_size_t size = sizeof(mach_msg_header_t);
    
    while (1) {
        mach_msg_header_t *msg = (mach_msg_header_t *)malloc(size);
        mach_msg_return_t result = mach_msg(msg, MACH_RCV_MSG, 0, size, port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
        
        if (result == KERN_SUCCESS) {
            // Callback-Funktion aufrufen
            my_callback(msg);
        }
        
        free(msg);
    }
    
    mach_port_deallocate(mach_task_self(), port);
    
    return 0;
}
//----------------------------------

#include <CoreFoundation/CoreFoundation.h>
#include <pthread.h>

static pthread_mutex_t lock;

void my_callback(CFRunLoopObserverRef observer, CFRunLoopActivity activity, void *info) {
    // Vermeiden Sie die Verwendung von lokal zugewiesenen Variablen
    // Vermeiden Sie komplexe Operationen
    // Verwenden Sie Mutexe, um sicherzustellen, dass das Callback nicht gleichzeitig von mehreren Threads ausgeführt wird
    pthread_mutex_lock(&lock);
    
    // Verwenden Sie Fehlerbehandlung, um sicherzustellen, dass das Callback ordnungsgemäß abgeschlossen wird
    // Debugging-Tools von macOS verwenden, um Probleme zu finden
    
    pthread_mutex_unlock(&lock);
}

int main() {
    pthread_mutex_init(&lock, NULL);
    
    CFRunLoopObserverRef observer = CFRunLoopObserverCreateWithHandler(kCFAllocatorDefault, kCFRunLoopAllActivities, true, 0, my_callback);
    
    CFRunLoopAddObserver(CFRunLoopGetMain(), observer, kCFRunLoopCommonModes);
    
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1.0, false);
    
    CFRunLoopRemoveObserver(CFRunLoopGetMain(), observer, kCFRunLoopCommonModes);
    CFRelease(observer);
    
    pthread_mutex_destroy(&lock);
    
    return 0;
}


*/


void
_callCFRunLoop(double seconds) {

    //printf("callCFRunLoop()\n");

    //this should start the message queue for the port, otherwise the data will hang
    //maybe use this? https://stackoverflow.com/questions/12825620/cfmessageport-weirdness
    //mode, seconds, returnAfterSourceHandled
    CFRunLoopRunResult runLoopRunReturnValue = CFRunLoopRunInMode(kCFRunLoopDefaultMode, seconds, true);

    if (runLoopRunReturnValue == kCFRunLoopRunHandledSource) {
        printf("C> niproto_pimpl::_callCFRunLoop: handled source\n");
/*
    } else if (runLoopRunReturnValue == kCFRunLoopRunFinished) {
        printf("C> niproto_pimpl::_callCFRunLoop: finished\n");
    } else if (runLoopRunReturnValue == kCFRunLoopRunStopped) {
        printf("C> niproto_pimpl::_callCFRunLoop: stopped\n");
    } else if (runLoopRunReturnValue == kCFRunLoopRunTimedOut) {
        //printf("C> niproto_pimpl::_callCFRunLoop: timeout\n");
*/
    } else {
        //TODO: check if this is an exception
        //printf("C> niproto_pimpl::_callCFRunLoop: Exception??? ERROR?\n");

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
