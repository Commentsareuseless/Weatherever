# Configuration file for example unit test

set(CORE_DIR ${CMAKE_SOURCE_DIR}/core)
set(CORE_INCLUDES
    "${GENERATED_FILES_DIR}/Core/Inc"
    "${GENERATED_FILES_DIR}/Drivers/STM32G0xx_HAL_Driver/Inc"
    "${GENERATED_FILES_DIR}/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy"
    "${GENERATED_FILES_DIR}/Drivers/CMSIS/Device/ST/STM32G0xx/Include"
    "${GENERATED_FILES_DIR}/Drivers/CMSIS/Include"
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/rtos/FreeRTOS/include"
    "${CMAKE_SOURCE_DIR}/rtos/FreeRTOS/portable/GCC/ARM_CM0/")

add_library(app_entry STATIC ${CORE_DIR}/Tasks.cpp)
target_include_directories(app_entry PRIVATE ${CORE_DIR} ${CORE_INCLUDES})
target_compile_definitions(
  app_entry PRIVATE CMSIS_device_header="<stm32g0xx.h>" USE_HAL_DRIVER
                    STM32G030xx UNIT_TEST)
target_link_libraries(app_entry PRIVATE etl::etl mock::freertos_api)

# Specify sources without .c postfix
set(TEST_SOURCES "TestAppRoot")

# Specify libraries to link with test
set(TEST_LIBS mock::freertos_api app_entry etl::etl)

# Specify include directories
set(TEST_DIRS ${CORE_DIR} ${CORE_INCLUDES} ${CMAKE_SOURCE_DIR}/test)

register_testcase(
  TC_LIST
  ${TEST_SOURCES}
  TEST_SRC_DIR
  ${CMAKE_CURRENT_LIST_DIR}
  LIBS
  ${TEST_LIBS}
  LINK_DIRS
  ${TEST_DIRS})
