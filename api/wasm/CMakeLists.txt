cmake_minimum_required(VERSION 3.6)

project(indigo-wasm)

#add_library(indigo-wasm SHARED indigo-wasm.c)
#target_link_libraries(indigo-wasm PUBLIC indigo-static)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s EXPORTED_RUNTIME_METHODS=['cwrap'] -s ALLOW_MEMORY_GROWTH=1 -s DISABLE_EXCEPTION_CATCHING=0 -s WASM_ASYNC_COMPILATION=0 -s EXPORTED_FUNCTIONS=['indigoAromatize','indigoVersion','indigoLoadMoleculeFromString','indigoMolfile'] -O3 --no-entry")
#set(CMAKE_SHARED_LIBRARY_SUFFIX ".js")
add_custom_target(${PROJECT_NAME} COMMAND emcc --profiling -s NODEJS_CATCH_EXIT=0 -s ASSERTIONS=1 -s DEMANGLE_SUPPORT=1 -s LIBRARY_DEBUG=1 -s EXCEPTION_DEBUG=1 -s EXPORTED_RUNTIME_METHODS=['cwrap'] -s INITIAL_MEMORY=167772160 -s DISABLE_EXCEPTION_THROWING=0 -s DISABLE_EXCEPTION_CATCHING=0 -s WASM_ASYNC_COMPILATION=0 --no-entry -Wl,--whole-archive $<TARGET_FILE:indigo-static> -Wl,--no-whole-archive $<TARGET_FILE:indigo-core> $<TARGET_FILE:inchi> $<TARGET_FILE:tinyxml> $<TARGET_FILE:zlib> -s EXPORTED_FUNCTIONS=['__embind_register_function','_indigoAromatize','_indigoDearomatize','_indigoVersion','_indigoLoadMoleculeFromString','_indigoMolfile','_indigoSmiles','_indigoGetLastError'] -o ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libindigo.js)
# -s MODULARIZE

# '_indigoClose','_indigoReleaseSessionId','_indigoSetSessionId','_indigoAllocSessionId',
# -s ALLOW_MEMORY_GROWTH=1 -s USE_PTHREADS=1


# DEMANGLE_SUPPORT LIBRARY_DEBUG

# -O3 -DNDEBUG -flto --llvm-opts 3
