cmake_minimum_required(VERSION 3.10)

project(Stack)
option(STACK_ENABLE_TESTING "Enable tests for stack" ON)

add_library(Stack INTERFACE)
target_include_directories(Stack INTERFACE includes)

if(STACK_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif(STACK_ENABLE_TESTING)

