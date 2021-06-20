// Rebellion
//
// File: foo.hpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020,2021
// License: LGPLv3

#ifndef _FOO_HPP_
#define _FOO_HPP_

#include <iostream>
#include <sstream>
#include <lua.hpp>

class Foo
{
public:
    Foo(const std::string & name);
    std::string Add(int a, int b); 
    const char * getName();
    ~Foo();
 
private:
    std::string name;
};
 
#endif
