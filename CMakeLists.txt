cmake_minimum_required(VERSION 3.6)

OPTION(TEST ON)
OPTION(BUILD_INDIGO OFF)
OPTION(BUILD_INDIGO_RENDERER OFF)
OPTION(BUILD_UTILS OFF)
OPTION(BUILD_BINGO_POSTGRES OFF)
OPTION(BUILD_BINGO_MSSQL OFF)
OPTION(BUILD_BINGO_ORACLE OFF)

project(indigo-all LANGUAGES C CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (TEST)
    enable_testing()
endif ()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/conan.cmake)

add_subdirectory(core/indigo-core)
if (BUILD_INDIGO_RENDERER OR BUILD_INDIGO_UTILS)
    add_subdirectory(core/render2d)
endif ()

if (BUILD_INDIGO OR BUILD_INDIGO_UTILS)
    add_subdirectory(api/indigo)
    add_subdirectory(api/indigo-inchi)
    add_subdirectory(api/bingo-nosql)
    if (BUILD_INDIGO_RENDERER OR BUILD_INDIGO_UTILS)
        add_subdirectory(api/indigo-renderer)
    endif ()
endif ()

if (BUILD_INDIGO_UTILS)
    add_subdirectory(utils/indigo-cano)
    add_subdirectory(utils/indigo-deco)
    add_subdirectory(utils/indigo-depict)
endif ()

if (BUILD_BINGO_POSTGRES OR BUILD_BINGO_ORACLE OR BUILD_BINGO_MSSQL)
    add_subdirectory(bingo/bingo-core)
    add_subdirectory(bingo/bingo-core-c)
endif()
