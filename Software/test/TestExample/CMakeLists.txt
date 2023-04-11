# Configuration file for example unit test

# Specify sources without .c postfix
set(TEST_SOURCES "TestExample" "TestThisShouldFail")

# Specify libraries to link with test
set(TEST_LIBS)

# Specify include directories
set(TEST_DIRS ${CMAKE_SOURCE_DIR}/ext/fff/)

register_testcase(
  TC_LIST
  ${TEST_SOURCES}
  TEST_SRC_DIR
  ${CMAKE_CURRENT_LIST_DIR}
  LIBS
  ${TEST_LIBS}
  LINK_DIRS
  ${TEST_DIRS})
