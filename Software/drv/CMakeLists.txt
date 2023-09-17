# Peripheral and external devices drivers

include(AddStaticLib)

add_static_lib(
  LIB_NAME
  peripherals
  LIB_NAMESPACE
  drv
  LIB_SRC
  ${CMAKE_CURRENT_LIST_DIR}/CortexM0Plus.cpp
  ${CMAKE_CURRENT_LIST_DIR}/PUsart.cpp
  LINK_LIBS
  etl::etl
  setup)

target_compile_definitions(
  peripherals PRIVATE CMSIS_device_header="<stm32g0xx.h>" USE_HAL_DRIVER
                      STM32G030xx)
target_include_directories(peripherals PRIVATE ${SETUP_INCLUDES})
