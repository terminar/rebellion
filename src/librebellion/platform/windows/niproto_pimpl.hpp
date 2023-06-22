// Rebellion
//
// File: niproto_pimpl.hpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#ifndef _NIPROTO_PIMPL_H_
#define _NIPROTO_PIMPL_H_

#include "../../niproto.hpp"
#include <windows.h>
#include <strsafe.h>

#include <string>

#define CONNECTING_STATE 0 
#define READING_STATE 1 
#define WRITING_STATE 2 
#define PIPE_TIMEOUT 5000
#define BUFSIZE 4096

typedef struct _PIPEINST
{
	OVERLAPPED oOverlap;
	HANDLE hPipeInst;
	char chRequest[BUFSIZE];
	DWORD cbRead;
	char chReply[BUFSIZE];
	DWORD cbToWrite;
	DWORD dwState;
	BOOL fPendingIO;
} PIPEINST, *LPPIPEINST;

class NIIPC::_NIIPC {
    public:
        _NIIPC(NIIPC *parent);
        ~_NIIPC();
        bool open();

        bool create();
        bool loop(double seconds);

        bool close();

        std::unique_ptr<NIIPC::Data> send(std::unique_ptr<NIIPC::Data> data);
        bool push(std::unique_ptr<NIIPC::Data> data);

    private:
        NIIPC *_parent;

        HANDLE _handle;
        std::string _name;

        PIPEINST _Pipe;
        HANDLE _hEvent;
};

#endif
