# Configuration of all test

# Configuration variables
set(PATH_TO_RUNNER_GENERATOR "${PROJECT_SOURCE_DIR}/ext/unity/auto")
set(UNITY_DIR "${PROJECT_SOURCE_DIR}/ext/unity")

macro(register_testcase)
  # directory where runners will be generated
  set(option_args OPTIONS)
  set(one_val_args TEST_SRC_DIR)
  set(multi_val_args TC_LIST LIBS LINK_DIRS)

  cmake_parse_arguments(ARG "${option_args}" "${one_val_args}"
                        "${multi_val_args}" ${ARGN})
  include_directories(${UNITY_DIR})

  foreach(test ${ARG_TC_LIST})
    add_custom_command(
      OUTPUT "${ARG_TEST_SRC_DIR}/${test}_runner.c"
      COMMAND
        "${PATH_TO_RUNNER_GENERATOR}/generate_test_runner.rb"
        "${ARG_TEST_SRC_DIR}/${test}.c" "${ARG_TEST_SRC_DIR}/${test}_runner.c")
    # add_custom_target(generate_runner DEPENDS
    # "${ARG_TEST_SRC_DIR}/${test}_runner.c")

    message("Created test " ${test})
    add_executable(${test} "${ARG_TEST_SRC_DIR}/${test}.c"
                           "${ARG_TEST_SRC_DIR}/${test}_runner.c")
    target_include_directories(
      ${test} PUBLIC ${PROJECT_SOURCE_DIR}/ext/unity/src ${ARG_LINK_DIRS})
    target_link_libraries(${test} PRIVATE ${ARG_LIBS} unity::framework)
    add_test(NAME ${test} COMMAND ${test})
  endforeach()
endmacro(register_testcase)

# Check if ruby is available (required by Unity test runners generator)
find_package(Ruby REQUIRED)

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
