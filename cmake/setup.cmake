find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    message(STATUS "Using ccache to increase compile speed: ${CCACHE_PROGRAM}")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif ()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (UNIX)
    string(APPEND CMAKE_C_FLAGS " -fvisibility=hidden")
    string(APPEND CMAKE_CXX_FLAGS " -fvisibility=hidden -fvisibility-inlines-hidden")
    if (BUILD_SELF_SUFFICIENT)
        string(APPEND CMAKE_CXX_FLAGS " -static-libstdc++")
    else ()
        list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    endif ()
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (ENABLE_TESTS)
    enable_testing()
endif ()

if (BUILD_SELF_SUFFICIENT)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/conan.cmake)
endif ()

find_package(Threads REQUIRED)

set(DIST_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)
set(INDIGO_NATIVE_LIBS_DIRECTORY ${DIST_DIRECTORY}/lib)

string(TOLOWER ${CMAKE_SYSTEM_NAME} CMAKE_SYSTEM_NAME_LOWER)
