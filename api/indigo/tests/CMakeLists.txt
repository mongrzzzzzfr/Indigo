cmake_minimum_required(VERSION 3.6)

project(indigo-tests LANGUAGES C CXX)

if (ENABLE_TESTS)
    add_executable(${PROJECT_NAME}-basic ${CMAKE_CURRENT_SOURCE_DIR}/indigo-test-basic.c)
    target_link_libraries(${PROJECT_NAME}-basic indigo)
    add_test(NAME ${PROJECT_NAME}-basic COMMAND ${PROJECT_NAME}-basic)
endif()
