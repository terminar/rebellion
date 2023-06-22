# Rebellion
#
# File: CMakeLists.txt
# Author: (C) Björn Kalkbrenner <terminar@cyberphoria.org> 2020-2023
# License: LGPLv3


#=== ---------------------------------------------------------------------------
message("nlohmann-json: HEADER")
configure_file(../nlohmann-json/single_include/nlohmann/json.hpp ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp COPYONLY)

set(TARGET nlohmann-json)
add_custom_target(${TARGET} DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp)
