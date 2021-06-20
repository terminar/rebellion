# Rebellion
#
# File: CMakeLists.txt
# Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020,2021
# License: LGPLv3

# === librebellion C(++) host === ----------------------------------------------
# get folders from other targets in this project
get_target_property(nlohmann-json_SOURCE_DIR nlohmann-json SOURCE_DIR)

#target_compile_definitions(${TARGET} PRIVATE "STANDALONE")

set(rebellion_host_HEADERS
    ${PROJECT_SOURCE_DIR}/include/rebellion.h
)

set(rebellion_host_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/host.cpp
)

set(rebellion_host_INCLUDEDIRS 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src-libs/nlohmann-json/single_include/nlohmann
)

#=== ---------------------------------------------------------------------------
set(TARGET rebellion_host)
add_executable(${TARGET}
    ${rebellion_host_HEADERS}
    ${rebellion_host_SOURCES}
)

target_include_directories(${TARGET} PRIVATE 
    ${rebellion_host_INCLUDEDIRS}
    ${nlohmann-json_SOURCE_DIR}
)

#target_link_libraries(${TARGET} librebellionStatic)
target_link_libraries(${TARGET} librebellion)

#install(TARGETS ${TARGET} 
#    RUNTIME DESTINATION ${REBELLION_HOME}
#    LIBRARY DESTINATION ${REBELLION_HOME}
#)
