// Rebellion
//
// File: niproto_pimpl.cpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020,2021
// License: LGPLv3

#include "niproto_pimpl.hpp"
#include "../../niproto.hpp"
#include "platform.hpp"

#include <iostream>
#include <string>

NIIPC::_NIIPC::_NIIPC(NIIPC *parent) 
    : _parent(parent),
      _handle(NULL),
      _hEvent(NULL) {
    this->_name = "\\\\.\\pipe\\" + _parent->getName();

  std::cout << MACRO_STR(PLATFORM_NAME) << "> Name: " << this->_name << std::endl;
}

NIIPC::_NIIPC::~_NIIPC() {
    if (this->_handle) {
        CloseHandle(this->_handle);
        this->_handle = NULL;
    }
}

bool NIIPC::_NIIPC::open() {
    BOOL fSuccess;
	DWORD dwMode;

    if (this->_handle) {
        std::cerr << "Open called but handle is set?!" << std::endl;
        return false;
    }
    this->_handle = createFile(this->_name.c_str());

    if (this->_handle == INVALID_HANDLE_VALUE) {
        std::cerr << "OPEN: Port creation failed: " << this->_name << std::endl;
    }
    else {
        std::cout << "OPEN: Port Creation success: " << this->_name << std::endl;
    }

    // The pipe connected; change to message-read mode. 
    dwMode = PIPE_READMODE_MESSAGE;
    fSuccess = SetNamedPipeHandleState(
        this->_handle,    // pipe handle 
        &dwMode,  // new pipe mode 
        NULL,     // don't set maximum bytes 
        NULL);    // don't set maximum time 
    if (!fSuccess)
    {
        std::cerr << "OPEN: SetNamedPipeHandleState failed" << std::endl;
        return false;
    }

    std::cout << "Returning handle..." << std::endl;
    return (this->_handle ? true : false);
}

bool NIIPC::_NIIPC::create() {
    std::cout << "CREATE: " << this->_name << std::endl;
    return createServer(&this->_Pipe, &this->_hEvent, this->_name.c_str());
}

bool NIIPC::_NIIPC::loop(double seconds) {
	DWORD i, dwWait, cbRet, dwErr;
	BOOL fSuccess;

    //std::cout << "loop" << std::endl;
    DWORD waitmili=0;
    if (seconds < 0) {
        waitmili = INFINITE;
    } else {
        waitmili = (DWORD) seconds * 1000;
    }    

    return loopServer(this->_parent, &this->_Pipe, &this->_hEvent, waitmili);
}

bool NIIPC::_NIIPC::close() {
    bool result = false;
    std::cout << "Trying to close " << this->_name << std::endl;
    if (this->_handle) {
        std::cout << "Closing handle" << std::endl;
        if (CloseHandle(this->_handle)) {
            result=true;
            std::cout << "Successfully closed handle" << std::endl;
        } else {
            std::cout << "FAILED closing handle" << std::endl;
            std::cout << "LastError: " << GetLastErrorStdStr() << std::endl;
        }
        this->_handle = NULL;
    }
    return result;
}

std::unique_ptr<NIIPC::Data> NIIPC::_NIIPC::send(std::unique_ptr<NIIPC::Data> data) {
    std::cout << ">>> SENDING TO " << this->_name << std::endl;
    uint8_t_data result = sendMsg(this->_handle, data->data(), data->size());

    if (result.data) {
        const uint8_t *ptr = static_cast<const uint8_t*>(result.data);
        auto res = std::make_unique<NIIPC::Data>(ptr,ptr+result.size);

        free(result.data);
        return res;
    }

    return nullptr;
}

bool NIIPC::_NIIPC::push(std::unique_ptr<NIIPC::Data> data) {
    std::cout << ">>> PUSHING TO " << this->_name << std::endl;
    int cnt = pushMsg(this->_handle, data->data(), data->size());
    return (data->size() == cnt ? true : false);
}
