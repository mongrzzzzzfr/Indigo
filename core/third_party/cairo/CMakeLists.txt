cmake_minimum_required(VERSION 3.4)

project(cairo C)

file(GLOB ${PROJECT_NAME}_SOURCES
        src/*.c
        boilerplate/*.c
        # util/*.c
)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})
target_include_directories(${PROJECT_NAME}
        PUBLIC src
        PRIVATE boilerplate
        # PRIVATE util
)
target_link_libraries(${PROJECT_NAME}
        PUBLIC pixman
        PUBLIC png)
target_compile_options(${PROJECT_NAME}
        PRIVATE -DHAVE_STDINT_H
        PRIVATE -DHAVE_UNISTD_H
        PRIVATE -DCAIRO_NO_MUTEX
        PRIVATE -DHAVE_UINT64_T)

message(STATUS "Using local version of ${PROJECT_NAME}")
