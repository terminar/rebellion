// Rebellion
//
// File: foo_l.cpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#include "foo.hpp"
#include "foo_l.h"

const static char *Foo_typename = "FooTypename";

static LUA_FUNC(l_Foo_destructor);
static LUA_FUNC(l_Foo_constructor);
static LUA_FUNC(l_Foo_add);
static LUA_FUNC(l_Foo_tostring);

static LUA_FUNC(l_Foo_pairs);

luaL_Reg sFooLibRegs[] =
{
    { "new", l_Foo_constructor },
    { NULL, NULL }
};

luaL_Reg sFooObjRegs[] =
{
    { "add", l_Foo_add },
    { "add2", l_Foo_add },
    { "add3", l_Foo_add },
    { NULL, NULL }
};

luaL_Reg sFooObjRegs_meta[] =
{
    { "__gc", l_Foo_destructor },
    { "__tostring", l_Foo_tostring },
    { "__pairs", l_Foo_pairs },
    { NULL, NULL }
};

luaL_IntTable sFooIntRegs[] = {
    { "MSG_VERSION", 0x01 },
    { "MSG_SERIAL", 0x02 },
    { "MSG_CONNECT", 0x03 },
    { NULL, 0 }
};

// The general pattern to binding C++ class to Lua is to write a Lua 
// thunk for every method for the class, so here we go:
 
int l_Foo_constructor(lua_State * L)
{
    const char * name = luaL_checkstring(L, 1);
 
    // We could actually allocate Foo itself as a user data but 
    // since user data can be GC'ed and we gain unity by using CRT's heap 
    // all along.
    Foo ** udata = (Foo **)lua_newuserdata(L, sizeof(Foo *));
    *udata = new Foo(name);
 
    // Usually, we'll just use "Foo" as the second parameter, but I 
    // say luaL_Foo here to distinguish the difference:
    //
    // This 2nd parameter here is an _internal label_ for luaL, it is 
    // _not_ exposed to Lua by default.
    //
    // Effectively, this metatable is not accessible by Lua by default.
    luaL_getmetatable(L, Foo_typename);
 
    // The Lua stack at this point looks like this:
    //     
    //     3| metatable "luaL_foo"   |-1
    //     2| userdata               |-2
    //     1| string parameter       |-3
    //
    // So the following line sets the metatable for the user data to the luaL_Foo 
    // metatable
    //
    // We must set the metatable here because Lua prohibits setting 
    // the metatable of a userdata in Lua. The only way to set a metatable 
    // of a userdata is to do it in C.
    lua_setmetatable(L, -2);
 
    // The Lua stack at this point looks like this:
    //     
    //     2| userdata               |-1
    //     1| string parameter       |-2
    // 
    // We return 1 so Lua callsite will get the user data and 
    // Lua will clean the stack after that.
 
    return 1;
}
 
Foo * l_CheckFoo(lua_State * L, int n)
{
    // This checks that the argument is a userdata 
    // with the metatable "luaL_Foo"
    return *(Foo **)luaL_checkudata(L, n, Foo_typename);
}

int l_Foo_add(lua_State * L)
{
    Foo * foo = l_CheckFoo(L, 1);
    int a = (int)luaL_checknumber(L, 2);
    int b = (int)luaL_checknumber(L, 3);
 
    std::string s = foo->Add(a, b);
    lua_pushstring(L, s.c_str());
 
    // The Lua stack at this point looks like this:
    //     
    //     4| result string          |-1
    //     3| metatable "luaL_foo"   |-2
    //     2| userdata               |-3
    //     1| string parameter       |-4
    //
    // Return 1 to return the result string to Lua callsite.
 
    return 1;
}

int l_Foo_destructor(lua_State * L)
{
    Foo * foo = l_CheckFoo(L, 1);
    delete foo;
 
    return 0;
}
 
int l_Foo_tostring (lua_State *L)
{
    Foo * foo = l_CheckFoo(L, 1);
    lua_pushfstring(L, "Foo: %s", foo->getName());
    return 1;
}

int l_Foo_pairs(lua_State * L)
{
    luaL_checkany(L, 1);

    printf("__pairs called\n");
    //stackDump(L);
    //printf("__pairs result\n");
    //lua_pushcfunction(L, l_pairsiter);
    //lua_pushvalue(L, -2);
    lua_pushcfunction(L, l_table_next);

    //funcnames table    
    lua_newtable(L);
    luaL_Reg *iter = sFooObjRegs;
    while (iter->name != NULL) {
        lua_pushcfunction(L, iter->func);
        lua_setfield(L,-2, iter->name);
        iter++;
    }

    luaL_IntTable *iiter = sFooIntRegs;
    while (iiter->name != NULL) {
        lua_pushinteger(L,iiter->Value);
        lua_setfield(L,-2, iiter->name);
        iiter++;
    }

    stackDump(L);
    
    /*
    Foo * foo = l_CheckFoo(L, 1);
    int a = (int)luaL_checknumber(L, 2);
    int b = (int)luaL_checknumber(L, 3);
 
    std::string s = foo->Add(a, b);
    lua_pushstring(L, s.c_str());
 */

    // The Lua stack at this point looks like this:
    //     
    //     4| result string          |-1
    //     3| metatable "luaL_foo"   |-2
    //     2| userdata               |-3
    //     1| string parameter       |-4
    //
    // Return 1 to return the result string to Lua callsite.
 
    return 2;
}
 
int luaopen_Foo(lua_State * L)
{
    printf(">>> Begin\n");
    stackDump(L);

    printf(">>> Add new metatable: %s\n", Foo_typename);
    //add type metatable
    luaL_newmetatable(L, Foo_typename);
	luaL_setfuncs (L, sFooObjRegs_meta, 0);

    printf(">>> Create func table: %s\n", Foo_typename);
    lua_newtable(L);
    luaL_setfuncs (L, sFooObjRegs, 0); //set function table

/*
    lua_pushinteger(L, 1);
    lua_setfield(L, -2, "MSG_VERSION");
*/
    luaL_IntTable *iter = sFooIntRegs;
    int i=1;
    while (iter->name != NULL) {
        lua_pushinteger(L,iter->Value);
        lua_setfield(L,-2, iter->name);
        iter++;
    }

    //set functable to metatable __index
    lua_setfield(L, -2, "__index");

    stackDump(L);
    lua_pop(L, 1); //metatable on stack


    stackDump(L);

    //create the library and attach the functions
    printf(">>> newlib\n");
    luaL_newlib(L, sFooLibRegs);

    stackDump(L);
 
    // The Lua stack at this point looks like this:
    //     
    //     1| metatable "luaL_Foo"   |-1
    //lua_pushvalue(L, -1);
 
    // The Lua stack at this point looks like this:
    //     
    //     2| metatable "luaL_Foo"   |-1
    //     1| metatable "luaL_Foo"   |-2
 
    // Set the "__index" field of the metatable to point to itself
    // This pops the stack
    //lua_setfield(L, -1, "__index");
 
    // The Lua stack at this point looks like this:
    //     
    //     1| metatable "luaL_Foo"   |-1
 
    // The luaL_Foo metatable now has the following fields
    //     - __gc
    //     - __index
    //     - add
    //     - new
 
    // Now we use setglobal to officially expose the luaL_Foo metatable 
    // to Lua. And we use the name "Foo".
    //
    // This allows Lua scripts to _override_ the metatable of Foo.
    // For high security code this may not be called for but 
    // we'll do this to get greater flexibility.
    //lua_setglobal(L, "Foo");
    return 1;
}
