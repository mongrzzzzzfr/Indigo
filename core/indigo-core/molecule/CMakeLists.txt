cmake_minimum_required(VERSION 3.4)

project(molecule LANGUAGES CXX)

file(GLOB CONFIGURE_DEPENDS ${CMAKE_PROJECT_NAME}_SOURCES src/*.cpp)
add_library(${CMAKE_PROJECT_NAME} STATIC ${CMAKE_PROJECT_NAME}_SOURCES)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${ PUBLIC common
                               PUBLIC CONAN_PKG::tinyxml
                               PUBLIC CONAN_PKG::rapidjson)

include_directories (../third_party/inchi/INCHI_BASE/src)
