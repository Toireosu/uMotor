add_executable(test_umotor tests.cc)
target_link_libraries(test_umotor PRIVATE umotor)
add_test(NAME test_umotor COMMAND test_umotor)
target_include_directories(test_umotor PRIVATE ${PROJECT_SOURCE_DIR}/include)