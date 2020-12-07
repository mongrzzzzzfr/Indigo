cmake_minimum_required(VERSION 3.4)

project(IndigoRenderer C CXX)

if (INDIGO_RENDERER_CONFIGURED)
	return()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ../../core/common/cmake/)

INCLUDE(ConfigureCommon)
add_subdirectory(../indigo "${CMAKE_CURRENT_BINARY_DIR}/indigo")
add_subdirectory(../../render2d "${CMAKE_CURRENT_BINARY_DIR}/render2d")
add_subdirectory(../../api/plugins/renderer "${CMAKE_CURRENT_BINARY_DIR}/renderer")

IF (PARENT_SCOPE)
	set(INDIGO_RENDERER_CONFIGURED TRUE PARENT_SCOPE)
ENDIF()
