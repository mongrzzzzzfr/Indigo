cmake_minimum_required(VERSION 3.6)

project(common LANGUAGES C CXX)

file(GLOB ${PROJECT_NAME}_SOURCES CONFIUGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/base_c/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/base_cpp/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gzip/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lzw/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/math/*.cpp)
# Collect file list for BaseC
if (MSVC OR MINGW)
    list(FILTER ${PROJECT_NAME}_SOURCES EXCLUDE REGEX .*posix\.cp*$)
else()
    list(FILTER ${PROJECT_NAME}_SOURCES EXCLUDE REGEX .*win32\.cp*$)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/conan.cmake)
conan_cmake_run(REQUIRES zlib/1.2.11
        BASIC_SETUP CMAKE_TARGETS
        BUILD missing)

add_library(common STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries(common PUBLIC CONAN_PKG::zlib)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
