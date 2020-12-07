cmake_minimum_required(VERSION 3.6)

project(indigo LANGUAGES C CXX)

find_package(Threads REQUIRED)

include(indigo-version.cmake)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/indigo_version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/indigo_version.h)

file(GLOB ${PROJECT_NAME}_SOURCES CONFIUGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}
        PUBLIC indigo-core
        PUBLIC Threads::Threads)
target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
