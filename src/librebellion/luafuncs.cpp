// Rebellion
//
// File: luafuncs.cpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#include "main.hpp"

#include "lua.hpp"
#ifdef __cplusplus
extern "C"
{
    #include "lua-struct.h"
    #include "lua-cjson.h"
}
#endif

#include <string>
#include <map>
#include <chrono>
#include <thread>
#include <sys/stat.h>

#include "foo_l.h"
#include "niproto_l.h"

#include <json.hpp>
using json = nlohmann::json;

static std::map<std::string, time_t> modifiedFiles;

#ifdef _WIN32
#define getcwd _getcwd
#endif

#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

// --- lua global ---
lua_State *L;
lua_State *L1;


int lua_usleep(lua_State *L)
{
    if (lua_isinteger(L, -1))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(lua_tointeger(L, -1)));
    }
    return 0;
}

int lua_sleep(lua_State *L)
{
    if (lua_isinteger(L, -1))
    {
        std::this_thread::sleep_for(std::chrono::seconds(lua_tointeger(L, -1)));
    }
    return 0;
}

int lua_cwd(lua_State *L)
{
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    lua_pushstring(L, cwd);
    return 1;
}

int l_table_next(lua_State *L)
{
    //printf("__pairsiter _next called\n");
    //stackDump(L);

    luaL_checktype(L, 1, LUA_TTABLE);
    lua_settop(L, 2); /* create a 2nd argument if there isn't one */
    if (lua_next(L, 1))
    {
        //printf("return 2\n");
        return 2;
    }
    else
    {
        //printf("return nil\n");
        lua_pushnil(L);
        return 1;
    }
}

static int lua_finish(lua_State *L)
{
    running = false;
    exit(1);
    return 0;
}

static int lua_rpc_callback(lua_State *L)
{
    DEBLf(DEB_DEBUG, "REBELLION LIB: rebellion lua_rpc_callback > start\n");

    if (lua_gettop(L) == 0) //request if callback is set
    {
        DEBLf(DEB_DEBUG, "REBELLION LIB: rebellion lua_rpc_callback > check if rpc callback is set\n");
        //        return luaL_error(L, "expecting exactly 2 arguments");
        lua_pushboolean(L, ( _rebellion_rpc_callback != NULL ? true : false) );
        return 1;
    }

    if (_rebellion_rpc_callback == NULL) {
        DEBLf(DEB_DEBUG, "REBELLION LIB: rebellion lua_rpc_callback > rpc callback is not set!\n");
        //        return luaL_error(L, "rebellion callback not set");
        lua_pushboolean(L, false);
        lua_pushstring(L, "rebellion callback not set");
        return 2;
    }

    const rebellion_message_format mf = (rebellion_message_format) luaL_checkinteger(L, 1);
    const rebellion_message_type mt = (rebellion_message_type) luaL_checkinteger(L, 2);
    const uint8_t *rpcdata = (uint8_t*)luaL_checkstring(L, 3);
    double rpcdata_len = luaL_checknumber(L, 4);

   DEBLf(DEB_DEBUG, "REBELLION LIB: rebellion lua_rpc_callback\n\t => sending '%s'\n", rpcdata);

    lua_pushnumber(L, _rebellion_rpc_callback(mf, mt, rpcdata, (uint32_t) rpcdata_len));
    return 1;
}

time_t fileModified(const char *filename)
{
    struct stat buf;
    int result;
    //errno_t err;
    bool ret = false;

    // Get data associated with "crt_stat.c":
    result = stat(filename, &buf);
    // Check if statistics are valid:
    if (result != 0)
    {
        perror("Problem getting information");
        switch (errno)
        {
        case ENOENT:
            DEBLf(DEB_ERROR, "File %s not found.\n", filename);
            break;
        case EINVAL:
            DEBLf(DEB_ERROR, "Invalid parameter to _stat.\n");
            break;
        default:
            /* Should never be reached. */
            DEBLf(DEB_ERROR, "Unexpected error in _stat.\n");
        }
    }
    else
    {
        return buf.st_mtime;
        /*
      // Output some of the statistics:
	  DEBf("C> mtime: %llu\n",buf.st_mtime);

		if (fileModified[filename] > 0 && fileModified[filename] < buf.st_mtime) {
			ret=true;
		} 
		fileModified[filename] = buf.st_mtime;
		*/
    }
    return -1;
}

bool loadLuaFile(lua_State *L, const char *filename)
{
    time_t modftime = fileModified(filename);
    time_t pmodftime = modifiedFiles[filename];
    int status = 0;

    if (pmodftime == 0 ||
        pmodftime < modftime)
    {
        if (pmodftime == 0)
        {
            DEBLf(DEB_INFO, "C> Loading file: %s\n", filename);
        }
        else
        {
            DEBLf(DEB_INFO, "C> Reloading file: %s\n", filename);
        }
        DEBLf(DEB_INFO, "C> pmodftime: %llu modftime: %llu\n", (unsigned long long)pmodftime, (unsigned long long)modftime);
        modifiedFiles[filename] = modftime;
        status = luaL_loadfile(L, filename);
        if (status)
        {
            DEBLf(DEB_ERROR, "C> could not load %s: %s\n", filename, lua_tostring(L, -1));
            exit(1);
        }
        return true;
    }

    return false;
}

void stackDump(lua_State *L)
{
    int i;
    int top = lua_gettop(L);
    printf("\nTop: %i\n============\n", top);

    for (i = 1; i <= top; i++)
    { /* repeat for each level */
        int t = lua_type(L, i);
        switch (t)
        {

        case LUA_TSTRING: /* strings */
            printf("%i| s:%s\n", i, lua_tostring(L, i));
            break;

        case LUA_TBOOLEAN: /* booleans */
            printf("%i| b:%s\n", i, lua_toboolean(L, i) ? "true" : "false");
            break;

        case LUA_TNUMBER: /* numbers */
            printf("%i| n:%g\n", i, lua_tonumber(L, i));
            break;

        default: /* other values */
            printf("%i| t:%s\n", i, lua_typename(L, t));
            break;
        }
        //printf("  ");  /* put a separator */
    }
    printf("\n\n"); /* end the listing */
}

void luaSetPreload(lua_State *L, const char *name, lua_CFunction f)
{
    lua_getfield(L, LUA_REGISTRYINDEX, "_PRELOAD");
    lua_pushcfunction(L, f);
    lua_setfield(L, -2, name);
    lua_pop(L, 1);
}

int luaopen_App(lua_State *L)
{
    lua_newtable(L);

    lua_pushstring(L, PATH_LOG);
    lua_setfield(L, -2, "PATH_LOG");

    lua_pushstring(L, PATH_SCRIPTS);
    lua_setfield(L, -2, "PATH_SCRIPTS");

    lua_pushcfunction(L, lua_usleep);
    lua_setfield(L, -2, "usleep");

    lua_pushcfunction(L, lua_sleep);
    lua_setfield(L, -2, "sleep");

    lua_pushcfunction(L, lua_cwd);
    lua_setfield(L, -2, "cwd");

    lua_pushcfunction(L, lua_rpc_callback);
    lua_setfield(L, -2, "rpc_callback");

    return 1;
}

int luaopen_RebellionTypes(lua_State *L) {
    lua_newtable(L);

    for (int i = 0; i < REBELLION_MF; i++) {
        lua_pushinteger(L, i);
        lua_setfield(L, -2, rebellion_message_format_str((rebellion_message_format) i));
    }

    for (int i = 0; i < REBELLION_MT; i++)
    {
        lua_pushinteger(L, i);
        lua_setfield(L, -2, rebellion_message_type_str((rebellion_message_type) i));
    }

    return 1;
}

// --- lua start / run / end ---
void luaStart()
{
    int status = 0;
    int result = 0;

    DEBLf(DEB_INFO, "C> pwd: %s\n", getcwd(NULL, 0));

    L = luaL_newstate();
    luaL_openlibs(L);

    luaSetPreload(L, "struct", luaopen_struct);
    luaSetPreload(L, "cjson", luaopen_cjson);
    luaSetPreload(L, "App", luaopen_App);
    luaSetPreload(L, "RebellionTypes", luaopen_RebellionTypes);
    luaSetPreload(L, "NIIPC", luaopen_NIIPC);

    luaSetPreload(L, "Foo", luaopen_Foo);

    loadLuaFile(L, PATH_LUASCRIPT_START);
    result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (result)
    {
        DEBLf(DEB_ERROR, "C> failed to run start.lua script: %s\n", lua_tostring(L, -1));
        exit(1);
    }
}

void luaInitL1()
{
    DEBLf(DEB_DEBUG, "C> luaInit()\n");
    L1 = lua_newthread(L);
    luaL_openlibs(L1);
    lua_register(L1, "finish", lua_finish);
}

void luaRunCoro()
{
    //	DEBf("C> \nluaRun START\n");
    int status = 0;
    int result = 0;

    if (loadLuaFile(L1, PATH_LUASCRIPT_PROC))
    {
        DEBLf(DEB_INFO, "C> changed (luaRunCoro\n");
        result = lua_pcall(L1, 0, LUA_MULTRET, 0);
        if (result != LUA_OK)
        {
            DEBLf(DEB_ERROR, "Error: %s\n", lua_tostring(L, -1));
        }
    }

    //lua_getglobal(L1, "run");

    do
    {
        /*
		DEBf("C> \ndo-loop\n");
		lua_pushnumber(L1, 2);
		lua_pushstring(L1, "hello-c");
		status = lua_resume(L1, NULL, 2, &result);
		*/
        status = lua_resume(L1, NULL, 0, &result);
        DEBLf(DEB_DEBUG, "C> lua_resume: %d\n", status);

        if (status == LUA_YIELD)
        {
            DEBLf(DEB_DEBUG, "C> lua yield status\n");
            lua_CFunction f = lua_tocfunction(L1, -1);
            f(L1);
        }
        if (status == LUA_ERRRUN)
        {
            DEBLf(DEB_ERROR, "Error: %s\n", luaL_checkstring(L1, 1));
        }
        DEBLf(DEB_DEBUG, "C> [luaRunCoro] do-loop end\n\n");

    } while (status == LUA_YIELD);

    //	DEBf("C> \nluaRun END\n");
}

void luaRun()
{
    //	DEBf("C> \nluaRun START\n");
    int status = 0;
    int result = 0;

    if (loadLuaFile(L, PATH_LUASCRIPT_PROC))
    {
        DEBLf(DEB_NOTE, "C> PROC changed (luaRun)\n");
        result = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (result != LUA_OK)
        {
            DEBLf(DEB_ERROR, "Error: %s\n", lua_tostring(L, -1));
        }
    }

    lua_getglobal(L, "run");

    result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (result != LUA_OK)
    {
        DEBLf(DEB_ERROR, "Error: %s\n", lua_tostring(L, -1));
    }

    DEBLf(DEB_DEBUG, "C> [luaRun] end\n\n");
    //std::this_thread::sleep_for(std::chrono::seconds(1));

    //	DEBf("C> \nluaRun END\n");
}

int luaRpc(rebellion_message_format mf, rebellion_message_type mt,
    const uint8_t *udata, uint32_t len) {
    DEBLf(DEB_DEBUG, "C> \nluaRpc START\n");
    int status = 0;
    int result = 0;

    if (loadLuaFile(L, PATH_LUASCRIPT_PROC))
    {
        DEBLf(DEB_NOTE, "C> PROC changed (luaRpc)\n");
        result = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (result != LUA_OK)
        {
            DEBLf(DEB_ERROR, "Error: %s\n", lua_tostring(L, -1));
        }
    }

    lua_getglobal(L, "rpc");
    lua_pushinteger(L, mf);
    lua_pushinteger(L, mt);
    lua_pushlstring(L, (const char*) udata, len);
    lua_pushinteger(L, len);
    result = lua_pcall(L, 4, LUA_MULTRET, 0);
    if (result != LUA_OK) {
        DEBLf(DEB_ERROR, "Error: %s\n", lua_tostring(L, -1));
    }

/*
    if (_rebellion_rpc_callback != NULL) {
        DEBf("C> luaRpc - calling callback function with result\n");
        std::string rpcret = R"(
            {
                "id":1,
                "result": true
            }
        )";
        _rebellion_rpc_callback(rpcret.c_str(), rpcret.length());
    }
*/
    //if some error, return error as negated result
    if (result > 0) {
        DEBLf(DEB_DEBUG, "C> \nluaRpc END (error)\n");
        return -result;
    }

    //result == 0, we have maybe an id
    if (lua_isinteger(L, -1))
    {
        int res = lua_tointeger(L, -1);
        if (res > 0) {
            result = res;
        }
    }
    DEBLf(DEB_DEBUG, "C> luaRpc END\n");

/*
LUA_ERRRUN: a runtime error.
LUA_ERRMEM: memory allocation error. For such errors, Lua does not call the error handler function.
LUA_ERRERR: error while running the error handler function.
*/
    return result;
}

void luaEnd()
{
    int status = 0;
    int result = 0;
    loadLuaFile(L, PATH_LUASCRIPT_END);
    result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (result)
    {
        DEBLf(DEB_ERROR, "C> failed to run end.lua script: %s\n", lua_tostring(L, -1));
        exit(1);
    }

    lua_close(L);
}
