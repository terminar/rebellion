
# === lua-cjson === ---------------------------------------------------------

set(lua-cjson_SOURCES
     ${CMAKE_CURRENT_SOURCE_DIR}/../lua-cjson/lua_cjson.c
     ${CMAKE_CURRENT_SOURCE_DIR}/../lua-cjson/strbuf.c
     ${CMAKE_CURRENT_SOURCE_DIR}/../lua-cjson/fpconv.c
)

set(lua-cjson_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/lua-cjson.h
     ${CMAKE_CURRENT_SOURCE_DIR}/../lua-cjson/fpconv.h
     ${CMAKE_CURRENT_SOURCE_DIR}/../lua-cjson/strbuf.h
)


# #=== ---------------------------------------------------------------------------
# message("liblua-cjson: SHARED")
# set(TARGET liblua-cjson)
# add_library(${TARGET} SHARED
#     ${lua-cjson_SOURCES}
#     ${lua-cjson_HEADERS}
# )

# get_target_property(liblua_SOURCE_DIR liblua SOURCE_DIR)
# target_include_directories(${TARGET}
#     PRIVATE "${liblua_SOURCE_DIR}/src"
# )

# set_target_properties(${TARGET} PROPERTIES PREFIX "")
# set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "liblua-cjson")

#=== ---------------------------------------------------------------------------
message("liblua-cjson: STATIC")
set(TARGET liblua-cjson-Static)
add_library(${TARGET} STATIC
    ${lua-cjson_SOURCES}
    ${lua-cjson_HEADERS}
)

get_target_property(liblua_SOURCE_DIR liblua SOURCE_DIR)
target_include_directories(${TARGET}
    PRIVATE "${liblua_SOURCE_DIR}/src"
)

set_target_properties(${TARGET} PROPERTIES PREFIX "")
set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "liblua-cjson")
