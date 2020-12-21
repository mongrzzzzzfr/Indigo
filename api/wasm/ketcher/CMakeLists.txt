cmake_minimum_required(VERSION 3.6)

project(indigo-ketcher)

add_library(${PROJECT_NAME} indigo-ketcher.cpp)
target_link_libraries(${PROJECT_NAME} indigo-static indigo-inchi-static)

set(EMCC_FLAGS
    -Oz -flto --closure 1
    --bind
    -s INITIAL_MEMORY=16777216
    -s DISABLE_EXCEPTION_THROWING=0
    -s DISABLE_EXCEPTION_CATCHING=0
    -s SINGLE_FILE=1
    -s MODULARIZE=1
    -s FILESYSTEM=0
    -s USE_SDL=0 -s USE_SDL_IMAGE=0 -s USE_SDL_TTF=0 -s USE_SDL_NET=0
    # -s DEMANGLE_SUPPORT=1
    # -s EXPORT_ALL=1
    --no-entry)
#if (CMAKE_BUILD_TYPE STREQUAL Release)
#    list(APPEND EMCC_FLAGS
#            -Oz
#            -flto
#            --closure 1)
#elseif (CMAKE_BUILD_TYPE STREQUAL Debug)
#    list(APPEND EMCC_FLAGS
#            -g3)
#endif()

add_custom_target(${PROJECT_NAME}-js
        COMMAND emcc
            -o ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libindigo-ketcher.js
            ${EMCC_FLAGS}
            -Wl,--whole-archive $<TARGET_FILE:indigo-ketcher> -Wl,--no-whole-archive
            $<TARGET_FILE:indigo-static> $<TARGET_FILE:indigo-inchi-static> $<TARGET_FILE:indigo-core> $<TARGET_FILE:inchi> $<TARGET_FILE:tinyxml> $<TARGET_FILE:zlib>
)

