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
# Indigo API Bindings options
OPTION(PREPARE_INDIGO_BINDINGS "Prepare native libraries folder for later building bindings. It could be prepared manually" OFF)
OPTION(BUILD_INDIGO_BINDINGS "Build all existing Indigo API bindings" OFF)
OPTION(BUILD_INDIGO_BINDINGS_PYTHON "Build Indigo API bindings to Python" OFF)
OPTION(BUILD_INDIGO_BINDINGS_JAVA "Build Indigo API bindings to Java" OFF)
OPTION(BUILD_INDIGO_BINDINGS_DOTNET "Build Indigo API bindings to .NET" OFF)
OPTION(BUILD_INDIGO_BINDINGS_R "Build Indigo API bindings to R" OFF)
# CLI utils options
OPTION(BUILD_UTILS "Build indigo-cano, indigo-deco and indigo-depict utilities" OFF)
# Bingo-SQL options
OPTION(BUILD_BINGO_POSTGRES "Build Bingo Postgres cartridge" OFF)
OPTION(BUILD_BINGO_MSSQL "Build Bingo MS SqlServer cartridge" OFF)
OPTION(BUILD_BINGO_ORACLE "Build Bingo Oracle cartridge" OFF)
# Auto-set dependable options
if (BUILD_UTILS AND NOT BUILD_INDIGO_RENDERER)
    message(STATUS "Enabling BUILD_INDIGO_RENDERER because it's required for BUILD_UTILS")
    set(BUILD_INDIGO_RENDERER ON)
    set(BUILD_INDIGO ON)
endif()
if (NOT BUILD_INDIGO)
    message(STATUS "Disabling BUILD_INDIGO_INCHI, BUILD_INDIGO_RENDERER, BUILD_BINGO_NOSQL and PREPARE_INDIGO_BINDINGS because BUILD_INDIGO=OFF")
    set(BUILD_INDIGO_INCHI OFF)
    set(BUILD_INDIGO_RENDERER OFF)
    set(BUILD_BINGO_NOSQL OFF)
    set(PREPARE_INDIGO_BINDINGS OFF)
endif()
if (BUILD_INDIGO_BINDINGS)
    message(STATUS "Enabling BUILD_INDIGO_BINDINGS_PYTHON, BUILD_INDIGO_BINDINGS_JAVA, BUILD_INDIGO_BINDINGS_DOTNET BUILD_INDIGO_BINDINGS_R because BUILD_INDIGO_BINDINGS=ON")
    set(BUILD_INDIGO_BINDINGS_PYTHON ON)
    set(BUILD_INDIGO_BINDINGS_JAVA ON)
    set(BUILD_INDIGO_BINDINGS_DOTNET ON)
    set(BUILD_INDIGO_BINDINGS_R ON)
endif()

# Set up compiler flags and other preparatory steps
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(setup)

# In subprojects we'll add dependencies for this pseudo-target to build all packages at once
add_custom_target(package)
add_subdirectory(core)
add_subdirectory(api)
if (BUILD_BINGO_POSTGRES OR BUILD_BINGO_ORACLE OR BUILD_BINGO_MSSQL)
    add_subdirectory(bingo)
endif()
if (BUILD_UTILS)
    add_subdirectory(utils)
endif()

# Print all options and settings
message(STATUS "ENABLE_TESTS=${ENABLE_TESTS}")
message(STATUS "BUILD_SELF_SUFFICIENT=${BUILD_SELF_SUFFICIENT}")
message(STATUS "BUILD_INDIGO=${BUILD_INDIGO}")
message(STATUS "BUILD_INDIGO_INCHI=${BUILD_INDIGO_INCHI}")
message(STATUS "BUILD_INDIGO_RENDERER=${BUILD_INDIGO_RENDERER}")
message(STATUS "BUILD_BINGO_NOSQL=${BUILD_BINGO_NOSQL}")
message(STATUS "PREPARE_INDIGO_BINDINGS=${PREPARE_INDIGO_BINDINGS}")
message(STATUS "BUILD_INDIGO_BINDINGS=${BUILD_INDIGO_BINDINGS}")
message(STATUS "BUILD_INDIGO_BINDINGS_PYTHON=${BUILD_INDIGO_BINDINGS_PYTHON}")
message(STATUS "BUILD_INDIGO_BINDINGS_JAVA=${BUILD_INDIGO_BINDINGS_JAVA}")
message(STATUS "BUILD_INDIGO_BINDINGS_DOTNET=${BUILD_INDIGO_BINDINGS_DOTNET}")
message(STATUS "BUILD_INDIGO_BINDINGS_R=${BUILD_INDIGO_BINDINGS_R}")
message(STATUS "BUILD_UTILS=${BUILD_UTILS}")
message(STATUS "BUILD_BINGO_POSTGRES=${BUILD_BINGO_POSTGRES}")
message(STATUS "BUILD_BINGO_MSSQL=${BUILD_BINGO_MSSQL}")
message(STATUS "BUILD_BINGO_ORACLE=${BUILD_BINGO_ORACLE}")
message(STATUS "CMAKE_C_FLAGS=${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")

find_package(R REQUIRED)
