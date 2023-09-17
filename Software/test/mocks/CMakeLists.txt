include(AddStaticLib)

add_static_lib(
  LIB_NAME
  "freertos_api"
  LIB_SRC
  "MockFreeRTOS.cpp"
  LIB_NAMESPACE
  "mock"
  LINK_LIBS
  GTest::gtest_main
  GTest::gmock
  LINK_DIRS
  "${CMAKE_SOURCE_DIR}/include"
  "${CMAKE_SOURCE_DIR}/rtos/FreeRTOS/include"
  "${CMAKE_SOURCE_DIR}/rtos/FreeRTOS/portable/GCC/ARM_CM0")
