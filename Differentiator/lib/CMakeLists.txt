cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0076 NEW)

list(APPEND DIFFERENTIATOR_SOURCES differentiator.c)
list(APPEND DIFFERENTIATOR_SOURCES Tree2Latex.c)
list(APPEND DIFFERENTIATOR_SOURCES treeCalculate.c)
list(APPEND DIFFERENTIATOR_SOURCES treeDump.c)
list(APPEND DIFFERENTIATOR_SOURCES treeParse.c)
list(APPEND DIFFERENTIATOR_SOURCES utilities.c)

add_library(DifferentiatorLib OBJECT)

target_sources(DifferentiatorLib PUBLIC ${DIFFERENTIATOR_SOURCES})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    string(REPLACE "/W3"  "/w" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
else ()
    target_compile_options(DifferentiatorLib PRIVATE -Wno-everything)
endif()

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GCC")
    target_compile_definitions(DifferentiatorLib PRIVATE M_PI=3.1415926)
endif ()


target_include_directories(DifferentiatorLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)