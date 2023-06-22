# Rebellion
#
# File: CMakeLists.txt
# Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
# License: LGPLv3

# === librebellion === ---------------------------------------------------------
# get folders from other targets in this project
get_target_property(lua_SOURCE_DIR liblua SOURCE_DIR)
get_target_property(lua-struct_SOURCE_DIR liblua-struct-Static SOURCE_DIR)
get_target_property(lua-cjson_SOURCE_DIR liblua-cjson-Static SOURCE_DIR)
get_target_property(nlohmann-json_SOURCE_DIR nlohmann-json SOURCE_DIR)


set(rebellion_HEADERS
    ${PROJECT_SOURCE_DIR}/include/rebellion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/main.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/foo.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/foo_l.h
    ${CMAKE_CURRENT_SOURCE_DIR}/niproto.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/niproto_l.h
    ${CMAKE_CURRENT_SOURCE_DIR}/luafuncs.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${PLATFORM_NAME}/platform.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${PLATFORM_NAME}/niproto_pimpl.hpp
)

set(rebellion_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/luafuncs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/foo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/foo_l.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/niproto.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/niproto_l.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${PLATFORM_NAME}/platform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${PLATFORM_NAME}/niproto_pimpl.cpp
)

set(rebellion_INCLUDEDIRS 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/librebellion
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${PLATFORM_NAME}
    ${lua_SOURCE_DIR}/src
    ${lua-struct_SOURCE_DIR}
    ${lua-cjson_SOURCE_DIR}
    ${nlohmann-json_SOURCE_DIR}
)

add_definitions(-D REBELLION_LIB)
if (WIN32)
    #add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    #add_definitions(-D_CRT_DEPRECATED_NO_WARNINGS)
    #add_definitions(-D_REENTRANT -D_POSIX_THREAD_SAFE_FUNCTIONS)
    #add_definitions(-DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=0x0501)
elseif(APPLE)
    set(rebellion_LIBS
        "-framework CoreFoundation"
    )
endif()
set(rebellion_LIBS 
    ${rebellion_LIBS} 
    liblua-Static
    liblua-struct-Static
    liblua-cjson-Static
)

#=== ---------------------------------------------------------------------------
message("librebellion: SHARED")
set(TARGET librebellion)
add_library(${TARGET} SHARED 
    ${rebellion_HEADERS} 
    ${rebellion_SOURCES}
)

set_target_properties(${TARGET} PROPERTIES PREFIX "")
if(WIN32)
    set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "rebellion")
else()
    set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "librebellion")
endif()

target_include_directories(${TARGET} PRIVATE 
    ${rebellion_INCLUDEDIRS}
)

target_link_libraries(${TARGET} ${rebellion_LIBS})

#=== ---------------------------------------------------------------------------
message("librebellion: STATIC")
set(TARGET librebellionStatic)
add_library(${TARGET} STATIC
    ${rebellion_HEADERS} 
    ${rebellion_SOURCES}
)

set_target_properties(${TARGET} PROPERTIES PREFIX "")
if (WIN32)
    set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "rebellion")
else()
    set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "librebellion")
endif()

target_include_directories(${TARGET} PRIVATE 
    ${rebellion_INCLUDEDIRS}
)

target_link_libraries(${TARGET} ${rebellion_LIBS})

#install(TARGETS librebellion librebellionStatic
#    RUNTIME DESTINATION ${REBELLION_HOME}
#    LIBRARY DESTINATION ${REBELLION_HOME}
#)
