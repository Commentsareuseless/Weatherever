# FreeRTOS internal cmake file. Do not use it in user top-level project

add_library(
  freertos_kernel_port STATIC
  # ARMv6-M port for GCC
  GCC/ARM_CM0/port.c)

target_include_directories(
  freertos_kernel_port PUBLIC # ARMv6-M port for GCC
                              ${CMAKE_CURRENT_LIST_DIR}/GCC/ARM_CM0)

target_link_libraries(freertos_kernel_port PRIVATE freertos_kernel)
