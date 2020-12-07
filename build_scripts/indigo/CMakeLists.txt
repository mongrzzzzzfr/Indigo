cmake_minimum_required(VERSION 3.4)

if (INDIGO_CONFIGURED)
    return()
endif()

project(IndigoSolution)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ../../core/common/cmake/)

INCLUDE(ConfigureCommon)

add_subdirectory(../../core/common "${CMAKE_CURRENT_BINARY_DIR}/common")

add_subdirectory(../../third_party/inchi "${CMAKE_CURRENT_BINARY_DIR}/third_party/inchi")
get_directory_property(InChI_HEADERS_DIR DIRECTORY ../../third_party/inchi DEFINITION InChI_HEADERS_DIR)

add_subdirectory(../../graph "${CMAKE_CURRENT_BINARY_DIR}/graph")
add_subdirectory(../../molecule "${CMAKE_CURRENT_BINARY_DIR}/molecule")
add_subdirectory(../../reaction "${CMAKE_CURRENT_BINARY_DIR}/reaction")
add_subdirectory(../../layout "${CMAKE_CURRENT_BINARY_DIR}/layout")
add_subdirectory(../../api "${CMAKE_CURRENT_BINARY_DIR}/api")

SET(INDIGO_VERSION "${INDIGO_VERSION}" PARENT_SCOPE)

set(INDIGO_CONFIGURED TRUE PARENT_SCOPE)
