# Rebellion
#
# File: CMakeLists.txt
# Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020,2021
# License: LGPLv3

# === fetch sources === --------------------------------------------------------
include(FetchContent)

#> fetch nlohmann-json
set(NLOHMANN_JSON_SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}/nlohmann-json")
file(GLOB RESULT "${NLOHMANN_JSON_SRCDIR}/")
list(LENGTH RESULT RES_LEN)
if(RES_LEN EQUAL 0)
    FetchContent_Declare(NLOHMANN-JSON
        GIT_REPOSITORY  https://github.com/nlohmann/json.git
    #    GIT_TAG 7126d88803eeb9d28cc10621f01a58813d50d078
        SOURCE_DIR "${NLOHMANN_JSON_SRCDIR}"
        GIT_PROGRESS true
    )
    FetchContent_GetProperties(NLOHMANN-JSON POPULATED NLOHMANN-JSON_POPULATED)
    if(NOT NLOHMANN-JSON_POPULATED)
        message("Fetching github.com::nlohmann/json")
        FetchContent_Populate(NLOHMANN-JSON)
        #FetchContent_MakeAvailable(NLOHMANN-JSON)
    #    add_subdirectory(${NLOHMANN-JSON_SOURCE_DIR} ${NLOHMANN-JSON_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
endif()

#> fetch openresty-lua-cjson
set(LUA_CJSON_SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}/lua-cjson")
file(GLOB RESULT "${LUA_CJSON_SRCDIR}/")
list(LENGTH RESULT RES_LEN)
if(RES_LEN EQUAL 0)
    FetchContent_Declare(OPENRESTY-LUA-CJSON
        GIT_REPOSITORY  https://github.com/openresty/lua-cjson.git
    #    GIT_TAG 891962b11d6d3b1b7275550b5c109e16c73ac94f
        SOURCE_DIR "${LUA_CJSON_SRCDIR}"
        GIT_PROGRESS true
    )
    FetchContent_GetProperties(OPENRESTY-LUA-CJSON POPULATED OPENRESTY-LUA-CJSON_POPULATED)
    if(NOT OPENRESTY-LUA-CJSON_POPULATED)
        message("Fetching github.com::openresty/lua-cjson")
        FetchContent_Populate(OPENRESTY-LUA-CJSON)
        #FetchContent_MakeAvailable(OPENRESTY-LUA-CJSON)
    endif()
endif()

add_subdirectory(lua)
add_subdirectory(lua-struct)
add_subdirectory(lua-cjson-custom)
add_subdirectory(nlohmann-json-custom)
