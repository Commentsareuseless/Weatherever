# Configuration file for example unit test
set(DRV_DIR ${CMAKE_SOURCE_DIR}/drv)

add_library(comm_hdl INTERFACE ${DRV_DIR}/CommHandler.hpp)
target_include_directories(comm_hdl INTERFACE ${DRV_DIR})
target_link_libraries(comm_hdl INTERFACE etl::etl)

# Specify sources without .c postfix
set(TEST_SOURCES "TestCommHandler")

# Specify libraries to link with test
set(TEST_LIBS comm_hdl)

# Specify include directories
set(TEST_DIRS ${DRV_DIR} ${CMAKE_SOURCE_DIR}/test)

register_testcase(
  TC_LIST
  ${TEST_SOURCES}
  TEST_SRC_DIR
  ${CMAKE_CURRENT_LIST_DIR}
  LIBS
  ${TEST_LIBS}
  LINK_DIRS
  ${TEST_DIRS})
