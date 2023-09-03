include(AddStaticLib)

set(COMMON_INCLUDES
    "${GENERATED_FILES_DIR}/Core/Inc"
    "${GENERATED_FILES_DIR}/Drivers/STM32G0xx_HAL_Driver/Inc"
    "${GENERATED_FILES_DIR}/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy"
    "${GENERATED_FILES_DIR}/Drivers/CMSIS/Device/ST/STM32G0xx/Include"
    "${GENERATED_FILES_DIR}/Drivers/CMSIS/Include"
    "${CMAKE_SOURCE_DIR}/include")

# Sources in Core
set(FILES_CORE "main.cpp" "SystemHooks.cpp")
# file( GLOB FILES_CORE LIST_DIRECTORIES FALSE
# "${CMAKE_CURRENT_LIST_DIR}/*")

# Not actual executable, we want to generate .elf
add_executable(
  ${PROJECT_NAME} ${GENERATED_FILES_DIR}/startup_stm32g030xx.s ${FILES_CORE}
                  ${FILES_DRIVERS} ${FILES_MIDDLEWARE})

target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDES})

target_compile_definitions(
  ${PROJECT_NAME} PRIVATE CMSIS_device_header="<stm32g0xx.h>" USE_HAL_DRIVER
                          STM32G030xx STM32G030xx)

set(LDSCRIPT "${GENERATED_FILES_DIR}/STM32G030C8Tx_FLASH.ld")
target_link_options(
  ${PROJECT_NAME}
  PRIVATE
  -T${LDSCRIPT}
  -mcpu=cortex-m0plus
  -mthumb
  -specs=nano.specs
  -lc
  -lm
  -lnosys
  -Wl,-Map=${PROJECT_NAME}.map,--cref
  -Wl,--gc-sections)

target_compile_options(
  ${PROJECT_NAME} PRIVATE -Wall -fdata-sections -ffunction-sections
                          -fno-exceptions)

target_link_libraries(${PROJECT_NAME} PRIVATE rtos setup)

# Print size of generated file
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_SIZE_UTIL} "--format=berkley --radix=16"
          "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}")

# Generate all needed artifacts
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  COMMAND ${CMAKE_OBJCOPY} "-Oihex" "${PROJECT_NAME}" "${PROJECT_NAME}.hex")

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  COMMAND ${CMAKE_OBJCOPY} "-Obinary" "${PROJECT_NAME}" "${PROJECT_NAME}.bin")

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  COMMAND ${CMAKE_OBJCOPY} "-Osrec" "${PROJECT_NAME}" "${PROJECT_NAME}.srec")
