
# === lua-struct === ---------------------------------------------------------

set(lua-struct_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/struct.c
)

set(lua-struct_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/lua-struct.h
)

# #=== ---------------------------------------------------------------------------
# message("liblua-struct: SHARED")
# set(TARGET liblua-struct)
# add_library(${TARGET} SHARED
#     ${lua-struct_SOURCES}
#     ${lua-struct_HEADERS}
# )

# get_target_property(liblua_SOURCE_DIR liblua SOURCE_DIR)
# target_include_directories(${TARGET}
#     PRIVATE "${liblua_SOURCE_DIR}/src"
# )

# set_target_properties(${TARGET} PROPERTIES PREFIX "")
# set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "liblua-struct")

#=== ---------------------------------------------------------------------------
message("liblua-struct: STATIC")
set(TARGET liblua-struct-Static)
add_library(${TARGET} STATIC
    ${lua-struct_SOURCES}
    ${lua-struct_HEADERS}
)

get_target_property(liblua_SOURCE_DIR liblua SOURCE_DIR)
target_include_directories(${TARGET}
    PRIVATE "${liblua_SOURCE_DIR}/src"
)

set_target_properties(${TARGET} PROPERTIES PREFIX "")
set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME "liblua-struct")
