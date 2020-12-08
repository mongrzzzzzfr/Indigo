cmake_minimum_required(VERSION 3.6)

project(indigo-all LANGUAGES C CXX)

# Options

# Common options
OPTION(ENABLE_TESTS "Enable CTest tests" ON)
OPTION(BUILD_SELF_SUFFICIENT "Build without any system dependencies except for libc, otherwise require tinyxml, zlib, rapidjson, and cairo for renderer" ON)
# Indigo API options
OPTION(BUILD_INDIGO "Build indigo shared library" ON)
OPTION(BUILD_INDIGO_INCHI "Build Indigo InChI plugin shared library" ON)
OPTION(BUILD_INDIGO_RENDERER "Build Indigo renderer plugin shared library" ON)
OPTION(BUILD_BINGO_NOSQL "Build Indigo bingo-nosql plugin shared library" ON)
# CLI utils options
OPTION(BUILD_UTILS "Build indigo-cano, indigo-deco and indigo-depict utilities" ON)
# Bingo-SQL options
OPTION(BUILD_BINGO_POSTGRES "Build Bingo Postgres cartidge" OFF)
OPTION(BUILD_BINGO_MSSQL "Build Bingo MS SqlServer cartidge" OFF)
OPTION(BUILD_BINGO_ORACLE "Build Bingo Oracle cartidge" OFF)
# Auto-set dependable options
if (BUILD_UTILS AND NOT BUILD_INDIGO_RENDERER)
    message(STATUS "Enabling BUILD_INDIGO_RENDERER because it's required for BUILD_UTILS")
    set(BUILD_INDIGO_RENDERER ON)
    set(BUILD_INDIGO ON)
endif()
if (NOT BUILD_INDIGO)
    message(STATUS "Disabling BUILD_INDIGO_INCHI, BUILD_INDIGO_RENDERER and BUILD_BINGO_NOSQL becaues BUILD_INDIGO=OFF")
    set(BUILD_INDIGO_INCHI OFF)
    set(BUILD_INDIGO_RENDERER OFF)
    set(BUILD_BINGO_NOSQL OFF)
endif()

# Set up compiler flags and other preparatory steps
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/setup.cmake)

add_subdirectory(core)
add_subdirectory(api)
add_subdirectory(bingo)
add_subdirectory(utils)

# Print all options and settings
message(STATUS "ENABLE_TESTS=${ENABLE_TESTS}")
message(STATUS "BUILD_SELF_SUFFICIENT=${BUILD_SELF_SUFFICIENT}")
message(STATUS "BUILD_INDIGO=${BUILD_INDIGO}")
message(STATUS "BUILD_INDIGO_INCHI=${BUILD_INDIGO_INCHI}")
message(STATUS "BUILD_INDIGO_RENDERER=${BUILD_INDIGO_RENDERER}")
message(STATUS "BUILD_BINGO_NOSQL=${BUILD_BINGO_NOSQL}")
message(STATUS "BUILD_UTILS=${BUILD_UTILS}")
message(STATUS "BUILD_BINGO_POSTGRES=${BUILD_BINGO_POSTGRES}")
message(STATUS "BUILD_BINGO_MSSQL=${BUILD_BINGO_MSSQL}")
message(STATUS "BUILD_BINGO_ORACLE=${BUILD_BINGO_ORACLE}")
message(STATUS "CMAKE_C_FLAGS=${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
