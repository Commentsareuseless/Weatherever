include(AddStaticLib)

# Sources in Core
file(
  GLOB FILES_CORE
  LIST_DIRECTORIES FALSE
  "${CMAKE_CURRENT_LIST_DIR}/*")

# Not actual executable, we want to generate .elf
add_executable(${PROJECT_NAME} ${GENERATED_FILES_DIR}/startup_stm32g030xx.s
                               ${FILES_CORE})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

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

target_link_libraries(${PROJECT_NAME} PRIVATE rtos drv::peripherals etl::etl)

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
