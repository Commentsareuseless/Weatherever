# Add test cases to compilation with this macro

macro(register_testcase)
  # Tag arguments for CMake parser
  set(option_args OPTIONS)
  set(one_val_args TEST_SRC_DIR)
  set(multi_val_args TC_LIST LIBS LINK_DIRS)

  cmake_parse_arguments(ARG "${option_args}" "${one_val_args}"
                        "${multi_val_args}" ${ARGN})

  foreach(test ${ARG_TC_LIST})
    add_executable(${test}
      "${ARG_TEST_SRC_DIR}/${test}.cpp"
      "${ARG_TEST_SRC_DIR}/TestMain.cpp")
    target_include_directories(${test} PUBLIC ${ARG_LINK_DIRS})
    target_link_libraries(${test} PRIVATE
      ${ARG_LIBS}
      CppUTest::CppUTest
      CppUTest::CppUTestExt)
    add_test(NAME ${test} COMMAND ${test})
    message(STATUS "Created test " ${test})
  endforeach()
endmacro(register_testcase)

