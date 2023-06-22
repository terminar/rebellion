// Rebellion
//
// File: niproto_pimpl.hpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#ifndef _NIPROTO_PIMPL_H_
#define _NIPROTO_PIMPL_H_

#include "../../niproto.hpp"

#include <CoreFoundation/CoreFoundation.h>

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
        
        CFMessagePortRef _portRef;
};

#endif
