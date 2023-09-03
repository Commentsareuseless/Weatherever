# All rtos configuration options should go here

set(FREERTOS_CONFIG_FILE_DIRECTORY
    ${CMAKE_SOURCE_DIR}/include/
    CACHE STRING "")
set(FREERTOS_PORT
    GCC_ARM_CM0
    CACHE STRING "")

# Only static allocation
set(FREERTOS_HEAP 0)

add_subdirectory(FreeRTOS)

add_library(rtos ALIAS freertos_kernel)
