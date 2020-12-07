cmake_minimum_required(VERSION 3.4)

project(bingo-core LANGUAGES CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/bingo-version.cmake)
message(STATUS "Bingo version is ${BINGO_VERSION}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/bingo_version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/bingo_version.h)

file(GLOB ${PROJECT_NAME}_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}
        PUBLIC indigo-core)
target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
