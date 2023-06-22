// Rebellion
//
// File: niproto_l.cpp
// Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
// License: LGPLv3

#include "niproto.hpp"
#include "niproto_l.h"

#include <map>
#include <cstring>

const static char *NIIPC_typename = "NIIPCTypename";

static LUA_FUNC(l_NIIPC_destructor);
static LUA_FUNC(l_NIIPC_constructor);
static LUA_FUNC(l_NIIPC_getName);
static LUA_FUNC(l_NIIPC_open);
static LUA_FUNC(l_NIIPC_create);
static LUA_FUNC(l_NIIPC_close);
static LUA_FUNC(l_NIIPC_send);
static LUA_FUNC(l_NIIPC_push);
static LUA_FUNC(l_NIIPC_loop);

static LUA_FUNC(l_NIIPC___tostring);
static LUA_FUNC(l_NIIPC___pairs);

static LUA_FUNC(l_NIIPC___index);
static LUA_FUNC(l_NIIPC___newindex);

std::map<std::string,lua_CFunction> l_NIIPCfuncs = {
    { "getName", l_NIIPC_getName },
    { "open", l_NIIPC_open },
    { "create", l_NIIPC_create },
    { "close", l_NIIPC_close },
    { "send", l_NIIPC_send },
    { "push", l_NIIPC_push },
    { "loop", l_NIIPC_loop }
};

//library function
luaL_Reg sNIIPCLibRegs[] =
{
    { "new", l_NIIPC_constructor },
    { NULL, NULL }
};

//userdata type metatable
luaL_Reg sNIIPCObjRegs_meta[] =
{
    { "__gc", l_NIIPC_destructor },
    { "__tostring", l_NIIPC___tostring },
    { "__pairs", l_NIIPC___pairs },

    { "__newindex", l_NIIPC___newindex },
    { "__index", l_NIIPC___index },

    { NULL, NULL }
};

struct CTX {
    NIIPC *object;
    lua_State *state;
    int callbackref;
};


//=== callback test ============================================================
static bool run_lua_callback(CTX *ctx, std::unique_ptr<NIIPC::Data> Data)
{
    if (!ctx || 
        ctx->callbackref < 0) {
            return false;
    }

    printf("C> run_lua_callback(): Trying to get function via callbackref\n");
    lua_rawgeti (ctx->state, LUA_REGISTRYINDEX, ctx->callbackref);
    /* Possibly push additional args here. */
    //TODO: data.
    int argsize = 1;
    if (Data && Data->size() > 0) {
        printf("C> run_lua_callback(): We have data, trying to push it to the stack\n");
        char *sresult = reinterpret_cast<char*>(Data->data());
        if (sresult) {
            lua_pushlstring(ctx->state, sresult, Data->size());
            lua_pushinteger(ctx->state, Data->size());
            argsize = 2;
        }
    } else {
        lua_pushnil(ctx->state);
        argsize = 1;
    }

    printf("C> run_lua_callback pre lua_call >>>>>>>>\n");
    lua_call (ctx->state, argsize, 1);
    printf("C> run_lua_callback post lua_call <<<<<<<<\n");
    return true;
}

static bool unregister_lua_callback(CTX *ctx) {
    printf("C> unregister_lua_callback(): trying to unregister lua callback\n");
    if (!ctx ||
        ctx->callbackref < 0) {
            return false;
    }

    ctx->object->setCallback(nullptr);

    //first, copy data
    int ref = ctx->callbackref;

    //free anchor to avoid race conditions - we have to detach everything
    ctx->callbackref = -1;

    //detach from registryindex to allow garbage collection of function
    luaL_unref(ctx->state, LUA_REGISTRYINDEX, ref);
    printf("C> unregister_lua_callback(): OK, unregistered and unreferenced\n");

    return true;
}


static bool register_lua_callback(CTX *ctx, int index)
{
    if (!ctx ||
        !lua_isfunction(ctx->state, index)) {
            return false;
    }

    printf("C> register_lua_callback(): trying to register lua callback\n");
    //callback set previously, unset first
    if (ctx->callbackref >= 0) {
        unregister_lua_callback(ctx);
    }

    //index here is assumed to be the stack position of the lua 
    //callback function.
    lua_pushvalue (ctx->state, index);
    ctx->callbackref = luaL_ref (ctx->state, LUA_REGISTRYINDEX);

    ctx->object->setCallback([ctx](std::unique_ptr<NIIPC::Data> data) -> std::unique_ptr<NIIPC::Data> {
        size_t datalen;
    
        printf("C> NIIPC callback called from lua\n");
        if (ctx->callbackref >= 0) {
            printf("C> Lua callback reference set, trying to run\n");
            //TODO: pass data to callback
            run_lua_callback(ctx, std::move(data));
            printf("C> Finished running callback, trying to check for data results\n");
            //if we have a string as result this should be returned back to
            //the caller (who may send it back)
            if (lua_isstring(ctx->state, -1)) {
                const char *s = (char*)luaL_checklstring(ctx->state, -1,&datalen);
                if (s) {
                    auto vdata = std::make_unique<NIIPC::Data>(s, s + datalen);
                    return vdata;
                }
            }

            printf("C> Done, no data, returning nullptr\n");
        }
        return nullptr;
    });

    //register_callback ((callback_function) callabck, (void *)c);
   printf("C> register_lua_callback(): Setting callback finished\n");
   return true;
}

int l_NIIPC_constructor(lua_State * L)
{
    const char * name = luaL_checkstring(L, 1);

    printf("C> NIIPC::Constructor: %s\n", name);
    /*
    NIIPC** udata = (NIIPC **)lua_newuserdata(L, sizeof(NIIPC *));
    *udata = new NIIPC(name);
    */

    struct CTX* ctx = (struct CTX*)lua_newuserdata(L, sizeof(*ctx));
    ctx->state = L;
    ctx->object = new NIIPC(name);
    ctx->callbackref = -1;

    //attach metatable from registry to userdata
    luaL_getmetatable(L, NIIPC_typename);
    lua_setmetatable(L, -2);

    return 1;
}

struct CTX* l_CheckNIIPC(lua_State * L, int n)
{
    // This checks that the argument is a userdata 
    // with the metatable "luaL_NIIPC"
    return (struct CTX*)luaL_checkudata(L, n, NIIPC_typename);
}

int l_NIIPC_destructor(lua_State * L)
{
    struct CTX *ctx = l_CheckNIIPC(L, 1);
    printf("C> NIIPC::Destructor: %s\n", ctx->object->getName().c_str());

    if (ctx->callbackref >= 0) {
        unregister_lua_callback(ctx);
    }

    delete ctx->object;
    ctx->object = NULL;
 
    return 0;
}

int l_NIIPC_getName(lua_State * L)
{
    struct CTX * ctx = l_CheckNIIPC(L, 1);
    lua_pushstring(L, ctx->object->getName().c_str());

    return 1;
}

int l_NIIPC_send(lua_State * L)
{
    size_t datalen;
    struct CTX * ctx = l_CheckNIIPC(L, 1);
    const char *s = (char*)luaL_checklstring(L, 2,&datalen);
 
    if (!s) {
        lua_pushnil(L);
        lua_pushstring(L, "Error, no data provided");
        return 2;
    }

    auto vdata = std::make_unique<NIIPC::Data>(s, s + datalen);
    auto result = ctx->object->send(std::move(vdata));

    if (!result || result->size() == 0) {
        lua_pushnil(L);
        lua_pushstring(L, "No result received");
        return 2;
    }

    char *sresult = reinterpret_cast<char*>(result->data());
    if (sresult) {
        lua_pushlstring(L, sresult, result->size());
        lua_pushinteger(L, result->size());
        return 2;
    }
  
    //or unknown
    lua_pushnil(L);
    return 1;
}

int l_NIIPC_push(lua_State * L)
{
    size_t datalen;
    struct CTX * ctx = l_CheckNIIPC(L, 1);
    const char *s = (char*)luaL_checklstring(L, 2,&datalen);
 
    if (!s) {
        lua_pushnil(L);
        lua_pushstring(L, "Error, no data provided");
        return 2;
    }

    auto vdata = std::make_unique<NIIPC::Data>(s, s + datalen);
    auto result = ctx->object->push(std::move(vdata));

    lua_pushboolean(L, result);
    return 1;
}

int l_NIIPC___tostring (lua_State *L)
{
    struct CTX * ctx = l_CheckNIIPC(L, 1);
    lua_pushfstring(L, "NIIPC: %s", ctx->object->getName().c_str());
    return 1;
}

int l_NIIPC___pairs(lua_State * L)
{
    luaL_checkany(L, 1);

    printf("C> __pairs called\n");
    lua_pushcfunction(L, l_table_next);

    lua_newtable(L);

    for (const auto& iter: l_NIIPCfuncs) {
        lua_pushcfunction(L, iter.second);
        lua_setfield(L,-2, iter.first.c_str());
    }

//    stackDump(L);
  
    return 2;
}

//setter
int l_NIIPC___newindex (lua_State *L)
{
    struct CTX * ctx = l_CheckNIIPC(L, 1);
    const char * name = luaL_checkstring(L, 2);

    printf("C> __newindex called: %s\n", name);
    if (name && strlen(name) == 8 && strncmp("callback", name, 8) == 0) {
        printf("C> callback field\n");
        if (lua_isnil(L, 3)) {
            unregister_lua_callback(ctx);
        } else if (lua_isfunction(L,3)) {
            register_lua_callback(ctx, 3);
        }
    }

    return 1;
}

//getter
int l_NIIPC___index (lua_State *L)
{
    struct CTX * ctx = l_CheckNIIPC(L, 1);
    //luaL_checktype(L, 1, LUA_TTABLE);
    const char * name = luaL_checkstring(L, 2);

    //printf("__index called: %s\n", name);
    
    if (name && strlen(name) == 8 && strncmp("callback", name, 8) == 0) {
        printf("C> callback field\n");
        if (ctx->callbackref >= 0) {
            lua_pushboolean(L, true);
        } else {
            lua_pushnil(L);
        }
        return 1;
    }

   //test with map
   auto iter = l_NIIPCfuncs.find(name);
   if (iter != l_NIIPCfuncs.end()) {
        //printf("C> returning function: %s\n", name);
        lua_pushcfunction(L, iter->second);
        return 1;
   }

    printf("C> Nothing found: %s\n", name);
    lua_pushnil(L);
    return 1;
}

int l_NIIPC_open (lua_State *L)
{
    struct CTX * ctx = l_CheckNIIPC(L, 1);
    lua_pushboolean(L, ctx->object->open());
    return 1;
}

int l_NIIPC_create (lua_State *L)
{
    struct CTX * ctx = l_CheckNIIPC(L, 1);
    lua_pushboolean(L, ctx->object->create());
    return 1;
}

int l_NIIPC_close (lua_State *L)
{
    struct CTX * ctx = l_CheckNIIPC(L, 1);
    lua_pushboolean(L, ctx->object->close());
    return 1;
}

int l_NIIPC_loop (lua_State *L)
{
    //printf("Trying to call _loop\n");
//    printf("-");
    struct CTX * ctx = l_CheckNIIPC(L, 1);
    double seconds = (lua_isnumber(L, 2) ? lua_tonumber(L, 2) : 0);

    //printf("Calling object loop\n");
//    printf("/");
    ctx->object->loop(seconds);
    //printf("Object loop called, returning 0\n");
//    printf("|");
    return 0;
}

int luaopen_NIIPC(lua_State * L)
{
    //add type metatable
    luaL_newmetatable(L, NIIPC_typename);

    //only set metatable functions
	luaL_setfuncs (L, sNIIPCObjRegs_meta, 0);

//    stackDump(L);
    lua_pop(L, 1); //metatable on stack


//    stackDump(L);

    //create the library and attach the functions
//    printf(">>> newlib\n");
    luaL_newlib(L, sNIIPCLibRegs);

    return 1;
}
