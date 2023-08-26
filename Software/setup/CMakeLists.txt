# Script for auto generated project

# TODO: Iterate through all directories recursively
# Add all found files to one executable
# link generated exe with rest of the project
# dun

set(GENERATED_FILES_DIR "${CMAKE_CURRENT_LIST_DIR}/WeathereverSTM")

# Remove auto generated FreeRTOS tasks and handlers, this is implemented
# at project root
file(REMOVE "${GENERATED_FILES_DIR}/Core/Src/freertos_app.c")

# Sources in Core
file(
  GLOB FILES_CORE
  LIST_DIRECTORIES FALSE
  "${GENERATED_FILES_DIR}/Core/Src/*")

# Sources in Drivers
file(
  GLOB FILES_DRIVERS
  LIST_DIRECTORIES FALSE
  "${GENERATED_FILES_DIR}/Drivers/STM32G0xx_HAL_Driver/Src/*")

# Sources in Middleware
file(
  GLOB FILES_MIDDLEWARE
  LIST_DIRECTORIES FALSE
  "${GENERATED_FILES_DIR}/Middlewares/Third_Party/FreeRTOS/Source/*.c"
  "${GENERATED_FILES_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/*.c"
  "${GENERATED_FILES_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/*.c")

# message(STATUS "Middleware files: ${FILES_MIDDLEWARE}")

# Not actual executable, we want to generate .elf
add_executable(${PROJECT_NAME}
  ${GENERATED_FILES_DIR}/startup_stm32g030xx.s
  ${FILES_CORE}
  ${FILES_DRIVERS}
  ${FILES_MIDDLEWARE})

target_include_directories(${PROJECT_NAME} PRIVATE
  "${GENERATED_FILES_DIR}/Core/Inc"
  "${GENERATED_FILES_DIR}/Drivers/STM32G0xx_HAL_Driver/Inc"
  "${GENERATED_FILES_DIR}/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy"
  "${GENERATED_FILES_DIR}/Middlewares/Third_Party/FreeRTOS/Source/include"
  "${GENERATED_FILES_DIR}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS"
  "${GENERATED_FILES_DIR}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0"
  "${GENERATED_FILES_DIR}/Drivers/CMSIS/Device/ST/STM32G0xx/Include"
  "${GENERATED_FILES_DIR}/Drivers/CMSIS/Include")

target_compile_definitions(${PROJECT_NAME} PRIVATE
  "CMSIS_device_header=<stm32g0xx.h>"
  "USE_HAL_DRIVER"
  "STM32G030xx")

set(LDSCRIPT "${GENERATED_FILES_DIR}/STM32G030C8Tx_FLASH.ld")
target_link_options(${PROJECT_NAME} PRIVATE
        -T${LDSCRIPT}
        -mcpu=cortex-m0plus
        -mthumb
        -specs=nano.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections)

target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall
  -fdata-sections
  -ffunction-sections)

# Print size of generated file
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_SIZE_UTIL}
  "--format=berkley --radix=16"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}")

# Generate all needed artifacts
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  COMMAND ${CMAKE_OBJCOPY}
  "-Oihex"
  "${PROJECT_NAME}"
  "${PROJECT_NAME}.hex")

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  COMMAND ${CMAKE_OBJCOPY}
  "-Obinary"
  "${PROJECT_NAME}"
  "${PROJECT_NAME}.bin")

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  COMMAND ${CMAKE_OBJCOPY}
  "-Osrec"
  "${PROJECT_NAME}"
  "${PROJECT_NAME}.srec")
