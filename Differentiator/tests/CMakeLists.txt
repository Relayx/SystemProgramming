cmake_minimum_required(VERSION 3.15)

add_executable(Test_Addition main_test.c)

target_link_libraries(Test_Addition PRIVATE DifferentiatorLib)

enable_testing()

add_test(Test_Addition Test_Addition)