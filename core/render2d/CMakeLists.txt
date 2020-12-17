cmake_minimum_required(VERSION 3.6)

project(render2d LANGUAGES C CXX)

file(GLOB ${PROJECT_NAME}_SOURCES CONFIUGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}
        PUBLIC indigo-core)

if (BUILD_SELF_SUFFICIENT)
    target_link_libraries(${PROJECT_NAME} PUBLIC cairo)
else()
    find_package(Cairo REQUIRED)
    target_link_libraries(${PROJECT_NAME}
            PUBLIC Cairo::Cairo)
endif()

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
