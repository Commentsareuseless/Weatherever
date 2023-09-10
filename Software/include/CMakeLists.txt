# Interface lib for FreeRTOS, to link configuration with kernel

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM
                           INTERFACE "${CMAKE_CURRENT_LIST_DIR}/")
target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)
