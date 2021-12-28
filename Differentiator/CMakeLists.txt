cmake_minimum_required(VERSION 3.10)

option(DF_ENABLE_TESTING "Enable testing for Differentiator lib" ON)

project(Differentiator)

add_subdirectory(lib)


if (DF_ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif ()
