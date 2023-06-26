# Configuration of all test

macro(register_testcase)
  # Tag arguments for CMake parser
  set(option_args OPTIONS)
  set(one_val_args TEST_SRC_DIR)
  set(multi_val_args TC_LIST LIBS LINK_DIRS)

  cmake_parse_arguments(ARG "${option_args}" "${one_val_args}"
                        "${multi_val_args}" ${ARGN})

  include(GoogleTest)

  foreach(test ${ARG_TC_LIST})
    message("Created test " ${test})
    add_executable(${test}
      "${ARG_TEST_SRC_DIR}/${test}.cpp")
    target_include_directories(${test} PUBLIC ${ARG_LINK_DIRS})
    target_link_libraries(${test} PRIVATE
      ${ARG_LIBS}
      GTest::gtest_main)
    add_test(NAME ${test} COMMAND ${test})
    gtest_discover_tests(${test})
  endforeach()
endmacro(register_testcase)

# Get subdirectories in test directory
file(
  GLOB FILES_AND_SUBDIRS
  LIST_DIRECTORIES TRUE
  "${CMAKE_CURRENT_LIST_DIR}/*")

# Add subdirectories with unit tests
foreach(dir ${FILES_AND_SUBDIRS})
  if(IS_DIRECTORY ${dir})
    add_subdirectory(${dir})
  else()
    continue()
  endif()
endforeach()
