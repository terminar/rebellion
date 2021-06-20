#!/usr/bin/env zsh

LUA_VERSION="5.4.3"

function _error() {
    if [ ! -z $* ] ; then
        echo "ERROR: $*"
    else
        echo "ERROR HAPPENED: Forcing termination of script..."
    fi
    kill -TERM $$
}

function TRAPTERM() {
    #TERM signal received, passthrough for the exit code
    return $((128+$1))
}

[ ! -e "lua-${LUA_VERSION}.tar.gz" ] && wget "http://www.lua.org/ftp/lua-${LUA_VERSION}.tar.gz"
rm -r ./src ./doc ./Makefile ./README
tar --strip-components=1 -xvjf "lua-${LUA_VERSION}.tar.gz" || _error "UNPACK failed"
#patch -p0 < patches/01_lua-5.4.2_shorthand-lambdas.patch || _error "SHORTHAND_LAMBDAS patch failed"
#patch -p0 < patches/02_defer_statement_for_Lua_5_4.patch || _error "DEFER patch failed"
