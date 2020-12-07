cmake_minimum_required(VERSION 3.6)

project(render2d LANGUAGES C CXX)

file(GLOB ${PROJECT_NAME}_SOURCES CONFIUGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/conan.cmake)
conan_cmake_run(REQUIRES
        cairo/1.17.2
        BASIC_SETUP CMAKE_TARGETS
        BUILD missing)
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}
        PUBLIC indigo-core
        PUBLIC CONAN_PKG::cairo)
target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
