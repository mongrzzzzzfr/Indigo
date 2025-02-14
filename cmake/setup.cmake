if(POLICY CMP0068)
    # https://cmake.org/cmake/help/v3.9/policy/CMP0068.html
    cmake_policy(SET CMP0068 NEW)
endif()
if (BUILD_BINGO_POSTGRES AND (WIN32 OR APPLE))
    # https://cmake.org/cmake/help/v3.12/policy/CMP0074.html
    cmake_policy(SET CMP0074 NEW)
endif()

if (BUILD_NATIVE)
    enable_language(C CXX)
endif()

if (NOT EMSCRIPTEN AND NOT CMAKE_HOST_WIN32)
    find_program(CCACHE_PROGRAM ccache)
    if (CCACHE_PROGRAM)
        message(STATUS "Using ccache to increase compile speed: ${CCACHE_PROGRAM}")
        set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    endif ()
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (EMSCRIPTEN)
    string(APPEND CMAKE_CXX_FLAGS " -c -fexceptions --bind -s DISABLE_EXCEPTION_CATCHING=0 -s FILESYSTEM=0 -s USE_SDL=0 -s USE_SDL_IMAGE=0 -s USE_SDL_TTF=0 -s USE_SDL_NET=0 -s")
    string(APPEND CMAKE_C_FLAGS   " -c -fexceptions --bind -s DISABLE_EXCEPTION_CATCHING=0 -s FILESYSTEM=0 -s USE_SDL=0 -s USE_SDL_IMAGE=0 -s USE_SDL_TTF=0 -s USE_SDL_NET=0 -s")

    set(CMAKE_CXX_FLAGS_RELEASE "-Oz -DNDEBUG -flto")
    set(CMAKE_C_FLAGS_RELEASE   "-Oz -DNDEBUG -flto")

    set(CMAKE_CXX_FLAGS_DEBUG "-g4")
    set(CMAKE_C_FLAGS_DEBUG   "-g4")

    set(CMAKE_AR "emar")
    set(CMAKE_C_CREATE_STATIC_LIBRARY   "<CMAKE_AR> qc <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_CXX_CREATE_STATIC_LIBRARY "<CMAKE_AR> qc <TARGET> <LINK_FLAGS> <OBJECTS>")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (UNIX OR MINGW)
    string(APPEND CMAKE_C_FLAGS " -fvisibility=hidden")
    string(APPEND CMAKE_CXX_FLAGS " -fvisibility=hidden -fvisibility-inlines-hidden")
    if (BUILD_STANDALONE AND NOT EMSCRIPTEN)
        if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
            string(APPEND CMAKE_CXX_FLAGS " -static-libstdc++ -static-libgcc")
        elseif  (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
            string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -stdlib=libc++")
        endif()
    endif ()

    if(MSYS OR CYGWIN OR MINGW)
        set(CMAKE_SHARED_LIBRARY_PREFIX "")
        if (MSYS OR CYGWIN)
            string(APPEND CMAKE_C_FLAGS " -D_GNU_SOURCE")
            string(APPEND CMAKE_CXX_FLAGS " -D_GNU_SOURCE")
        endif()
    endif()
elseif (MSVC)
    string(APPEND CMAKE_C_FLAGS " -MP -D_CRT_SECURE_NO_WARNINGS -D_WIN32_WINNT=0x0601 -DWINVER=0x0601")
    string(APPEND CMAKE_CXX_FLAGS " -MP -EHs -D_CRT_SECURE_NO_WARNINGS -D_WIN32_WINNT=0x0601 -DWINVER=0x0601")
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endforeach()

string(TOLOWER ${CMAKE_SYSTEM_NAME} CMAKE_SYSTEM_NAME_LOWER)
string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} CMAKE_SYSTEM_PROCESSOR_LOWER)
if (CMAKE_SYSTEM_PROCESSOR_LOWER STREQUAL "amd64")
    set(CMAKE_SYSTEM_PROCESSOR_LOWER "x86_64")
endif()
if (CMAKE_SYSTEM_NAME_LOWER STREQUAL "msys")
    set(CMAKE_SYSTEM_NAME_LOWER "windows")
endif()

if (ENABLE_TESTS)
    set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
    enable_testing()
endif ()

set(DIST_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)
set(INDIGO_NATIVE_LIBS_DIRECTORY ${DIST_DIRECTORY}/lib)
set(INDIGO_REPOSITORY_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Show C and CXX flags
message(STATUS "CMAKE_C_FLAGS=${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
if (CMAKE_BUILD_TYPE STREQUAL Debug)
    message(STATUS "CMAKE_C_FLAGS_DEBUG=${CMAKE_C_FLAGS_DEBUG}")
    message(STATUS "CMAKE_CXX_FLAGS_DEBUG=${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
    message(STATUS "CMAKE_C_FLAGS_RELEASE=${CMAKE_C_FLAGS_RELEASE}")
    message(STATUS "CMAKE_CXX_FLAGS_RELEASE=${CMAKE_CXX_FLAGS_RELEASE}")
endif()
message(STATUS "CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")

# Fix problem with CMake not supporting copy with wildcards on windows
# Use like ${COPY_COMMAND} ${SEP}some${SEP}path${sep}file.txt ${NATIVE_DIST_DIRECTORY}${SEP}
if (WIN32)
    find_program(CP cp PATHS ENV{PATH})
    if (CP)
        set(COPY_COMMAND cp)
    else()
        set(COPY_COMMAND copy)
    endif()
else()
    set(COPY_COMMAND cp)
endif()
file(TO_NATIVE_PATH "/" SEP)
file(TO_NATIVE_PATH ${DIST_DIRECTORY} NATIVE_DIST_DIRECTORY)
