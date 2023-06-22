# Rebellion
#
# File: CMakeLists.txt
# Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
# License: LGPLv3

cmake_minimum_required (VERSION 3.13.3)

project(REBELLION)

#set(CMAKE_VERBOSE_MAKEFILE ON)

# === general cmake ≤settings === ----------------------------------------------
if(WIN32)
    set(PLATFORM_NAME "windows")
    # disable deprecation warnings (like replace fopen with fopen_s)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
    set(PLATFORM_NAME "macos")
endif()
add_definitions(-DPLATFORM_NAME=${PLATFORM_NAME})

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message("Install prefix: initialized to default > ${CMAKE_INSTALL_PREFIX}")
    if(WIN32)
        message("Install prefix: Setting install prefix to Home:$ENV{APPDATA}")
        set (CMAKE_INSTALL_PREFIX "$ENV{APPDATA}/" CACHE PATH "default install path" FORCE ) 
    elseif (APPLE)
        message("Install prefix: Setting install prefix to Home:$ENV{HOME}/Library")
        set (CMAKE_INSTALL_PREFIX "$ENV{HOME}/Library" CACHE PATH "default install path" FORCE ) 
    endif()
endif()
message("Install prefix: ${CMAKE_INSTALL_PREFIX}")

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    message("debug mode")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0") 
else(CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRELEASEVERSION")
endif(CMAKE_BUILD_TYPE MATCHES DEBUG) 

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# on windows, cmake will produce a differnt folder structure. 
# we try to force the same folder cross platform
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src-vendor)
add_subdirectory(src)
