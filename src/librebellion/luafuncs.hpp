// Rebellion
//
// File: luafuncs.hpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#ifndef _LUA_H__
#define _LUA_H__

#include <lua.hpp>
#include <rebellion.h>

//--- Lua
#define LUA_FUNC(fname) int fname(lua_State * L)

typedef struct luaL_IntTable {
  const char *name;
  int Value;
} luaL_IntTable;

void luaStart();
void luaInitL1();
void luaRun();
int luaRpc(rebellion_message_format mf, rebellion_message_type mt, const uint8_t *udata, uint32_t len);
void luaEnd();

void stackDump (lua_State *L);
void luaSetPreload(lua_State *L,const char *name, lua_CFunction f);


//Lua helper
int l_table_next(lua_State *L);

#endif
