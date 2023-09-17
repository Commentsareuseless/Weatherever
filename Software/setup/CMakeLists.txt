# Script for auto generated project

# Sources in Core
file(
  GLOB SETUP_CORE_SRC
  LIST_DIRECTORIES FALSE
  "${GENERATED_FILES_DIR}/Core/Src/*")

# Sources in Drivers
file(
  GLOB SETUP_DRV_SRC
  LIST_DIRECTORIES FALSE
  "${GENERATED_FILES_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/*")

set(SETUP_LIB_NAME "setup")

# Not actual executable, we want to generate .elf
add_library(${SETUP_LIB_NAME} ${GENERATED_FILES_DIR}/startup_stm32g030xx.s
                              ${SETUP_CORE_SRC} ${SETUP_DRV_SRC})

target_include_directories(
  ${SETUP_LIB_NAME}
  PUBLIC "${GENERATED_FILES_DIR}/Core/Inc"
         "${GENERATED_FILES_DIR}/Drivers/STM32G0xx_HAL_Driver/Inc"
         "${GENERATED_FILES_DIR}/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy"
         "${GENERATED_FILES_DIR}/Drivers/CMSIS/Device/ST/STM32G0xx/Include"
         "${GENERATED_FILES_DIR}/Drivers/CMSIS/Include")

target_compile_definitions(
  ${SETUP_LIB_NAME} PRIVATE "CMSIS_device_header=<stm32g0xx.h>"
                            "USE_HAL_DRIVER" "STM32G030xx")

set(LDSCRIPT "${GENERATED_FILES_DIR}/STM32G030C8Tx_FLASH.ld")
target_link_options(
  ${SETUP_LIB_NAME}
  PRIVATE
  -T${LDSCRIPT}
  -mcpu=cortex-m0plus
  -mthumb
  -specs=nano.specs
  -lc
  -lm
  -lnosys
  -Wl,-Map=${SETUP_LIB_NAME}.map,--cref
  -Wl,--gc-sections)

target_compile_options(${SETUP_LIB_NAME} PRIVATE -Wall -fdata-sections
                                                 -ffunction-sections)
