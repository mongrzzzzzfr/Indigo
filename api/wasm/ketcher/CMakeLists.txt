cmake_minimum_required(VERSION 3.6)

project(indigo-wasm)

#if (CMAKE_BUILD_TYPE STREQUAL Debug)
#    set(INDIGO_EMSCRIPTEN_FLAGS " -g")
#elseif (CMAKE_BUILD_TYPE STREQUAL Release)
#    set(INDIGO_EMSCRIPTEN_FLAGS " -Oz")
#endif()

message(STATUS "INDIGO_EMSCRIPTEN_FLAGS=${INDIGO_EMSCRIPTEN_FLAGS}")
add_custom_target(${PROJECT_NAME}
                  COMMAND emcc
                    # -Oz -flto -DNDEBUG  # --closure 1
                    -Oz -flto --closure 1
                    --bind
                    -s INITIAL_MEMORY=16777216
                    -s EXPORTED_RUNTIME_METHODS=['cwrap']
                    -s DISABLE_EXCEPTION_THROWING=0
                    -s DISABLE_EXCEPTION_CATCHING=0
                    -s SINGLE_FILE=1
                    -s MODULARIZE=1
                    -s FILESYSTEM=0
                    -s USE_SDL=0 -s USE_SDL_IMAGE=0 -s USE_SDL_TTF=0 -s USE_SDL_NET=0
                    -s EXPORTED_FUNCTIONS=['_indigoAromatize','_indigoDearomatize','_indigoVersion','_indigoLoadMoleculeFromString','_indigoMolfile','_indigoSmiles','_indigoGetLastError']
                    --no-entry
                    -Wl,--whole-archive $<TARGET_FILE:indigo-static> -Wl,--no-whole-archive $<TARGET_FILE:indigo-core> $<TARGET_FILE:inchi> $<TARGET_FILE:tinyxml> $<TARGET_FILE:zlib>
                    -o ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libindigo.js
                  )

# -s MODULARIZE
# '_indigoClose','_indigoReleaseSessionId','_indigoSetSessionId','_indigoAllocSessionId',
# -s ALLOW_MEMORY_GROWTH=1 -s USE_PTHREADS=1
# --profiling -s NODEJS_CATCH_EXIT=0 -s ASSERTIONS=1 -s DEMANGLE_SUPPORT=1 -s LIBRARY_DEBUG=1  -s EXCEPTION_DEBUG=1
# -O3 -DNDEBUG -flto --llvm-opts 3
