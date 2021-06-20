
set(lua_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lapi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lauxlib.c    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lbaselib.c  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lcode.c    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lcorolib.c   
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lctype.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ldblib.c     
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ldebug.c     
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ldo.c        
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ldump.c      
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lfunc.c      
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lgc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/linit.c      
    ${CMAKE_CURRENT_SOURCE_DIR}/src/liolib.c     
    ${CMAKE_CURRENT_SOURCE_DIR}/src/llex.c       
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lmathlib.c   
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lmem.c       
    ${CMAKE_CURRENT_SOURCE_DIR}/src/loadlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lobject.c    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lopcodes.c   
    ${CMAKE_CURRENT_SOURCE_DIR}/src/loslib.c     
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lparser.c    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lstate.c     
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lstring.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lstrlib.c    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ltable.c     
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ltablib.c    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ltm.c        
    #	${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c        
    #	${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lundump.c    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lutf8lib.c   
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lvm.c        
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lzio.c
)

set(lua_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lapi.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ldebug.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ljumptab.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lprefix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ltm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lundump.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lauxlib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ldo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/llex.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lopcodes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lstate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lvm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lcode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lfunc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/llimits.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lopnames.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lstring.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/luaconf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lzio.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lctype.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lgc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lmem.h	
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lparser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ltable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lualib.h
)

#=== ---------------------------------------------------------------------------
message("liblua: SHARED")
set(TARGET liblua)
add_library(${TARGET} SHARED 
    ${lua_HEADERS} 
    ${lua_SOURCES}
)

set_target_properties(${TARGET} PROPERTIES PREFIX "")
set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "liblua")

target_include_directories(${TARGET} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

#=== ---------------------------------------------------------------------------
message("liblua: STATIC")
set(TARGET liblua-Static)
add_library(${TARGET} STATIC
    ${lua_HEADERS} 
    ${lua_SOURCES}
)

set_target_properties(${TARGET} PROPERTIES PREFIX "")
set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "liblua")

target_include_directories(${TARGET} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
