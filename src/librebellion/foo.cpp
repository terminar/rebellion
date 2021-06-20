// Rebellion
//
// File: foo.cpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020,2021
// License: LGPLv3

#include "foo.hpp"
 
Foo::Foo(const std::string & name) : name(name)
{
    std::cout << "Foo is born" << std::endl;
}

std::string Foo::Add(int a, int b)
{
    std::stringstream ss;
    ss << name << ": " << a << " + " << b << " = " << (a+b);
    return ss.str();
}

const char * Foo::getName() {
    return name.c_str();
}

Foo::~Foo()
{
    std::cout << "Foo is gone" << std::endl;
}
