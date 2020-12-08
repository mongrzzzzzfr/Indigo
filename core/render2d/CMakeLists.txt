cmake_minimum_required(VERSION 3.6)

project(render2d LANGUAGES C CXX)

file(GLOB ${PROJECT_NAME}_SOURCES CONFIUGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})

if (BUILD_SELF_SUFFICIENT)
    conan_cmake_run(REQUIRES
            cairo/1.17.2
            OPTIONS
                cairo:shared=False
                cairo:with_xlib=False
                cairo:with_xlib_xrender=False
                cairo:with_xcb=False
                cairo:with_glib=False
            BASIC_SETUP CMAKE_TARGETS
            BUILD missing)
    target_link_libraries(${PROJECT_NAME}
            PUBLIC indigo-core
            PUBLIC CONAN_PKG::cairo)
else()
    find_package(Cairo REQUIRED)
    target_link_libraries(${PROJECT_NAME}
            PUBLIC indigo-core
            PUBLIC Cairo::Cairo)
endif()

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
