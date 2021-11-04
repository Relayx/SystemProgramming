message(STATUS "[STACK] create Test")

#TODO: if you will enable dumping, it will broke everything
option(STACK_TEST_ENABLE_DUMPING "Enable stack dump" OFF)
add_executable(Test stackTests.c)

if(STACK_TEST_ENABLE_DUMPING)
target_compile_definitions(Test PRIVATE STACK_DUMPING)
endif(STACK_TEST_ENABLE_DUMPING)

target_link_libraries(Test PRIVATE Stack)
add_test(Test Test)