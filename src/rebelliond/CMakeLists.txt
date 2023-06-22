# Rebellion
#
# File: CMakeLists.txt
# Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
# License: LGPLv3

# Go cmake configuration
cmake_minimum_required(VERSION 3.0)

#find_library(golang REQUIRED)

set(TARGET rebellion_daemon)
set(BIN_NAME rebelliond)
set(GOPATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#set(CMAKE_GO_FLAGS "${CMAKE_GO_FLAGS} --ldflags '-extldflags \"-static\"'")
#message(${CMAKE_GO_FLAGS})
set(rebelliond_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/rebelliond.go
    ${CMAKE_CURRENT_SOURCE_DIR}/oscd.go
    ${CMAKE_CURRENT_SOURCE_DIR}/rebellion_api.go
)

#go build -ldflags="-extldflags=-static"
#set GOOS=windows set GOARCH=amd64 set CGO_ENABLED=1
if(WIN32)
    add_custom_command(OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BIN_NAME}
        DEPENDS librebellion ${rebelliond_SRCS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} 
            -E env 
            "CGO_CFLAGS=-I${PROJECT_SOURCE_DIR}/include" 
            "CGO_LDFLAGS=-L${CMAKE_LIBRARY_OUTPUT_DIRECTORY} -L${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" 
        go build -x
            -o "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BIN_NAME}.exe"
        ${CMAKE_GO_FLAGS}
        COMMENT "Building Rebellion daemon - Windows"
    )
elseif(APPLE)
    add_custom_command(OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BIN_NAME}
        DEPENDS librebellion ${rebelliond_SRCS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} 
            -E env 
            "CGO_CFLAGS=-I ${PROJECT_SOURCE_DIR}/include" 
            "CGO_LDFLAGS=-L ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} -Wl,-rpath,./lib,-rpath,./" 
            go build
            -o "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BIN_NAME}"
        ${CMAKE_GO_FLAGS}
        COMMENT "Building Rebellion daemon - MacOS"
    )
endif()

add_custom_target(${TARGET} ALL
    DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BIN_NAME})
