# This file exist because setting 10000000 variables in main cmake file makes
# mess and I don't like too much mess :)

# All pre-project variables should go here

# ##############################################################################
# Project versioning
# ##############################################################################
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 1)

# ##############################################################################
# Platform related
# ##############################################################################

# Target names, use these variables instead of names directly
set(TARGET_STM32G0 "stm32g0xx")
set(TARGET_NUCLEO_F1 "stm32f1xx")
set(TARGET_DESKTOP "native-test")

# This variable controlls what platform we're compiling project for. Possible
# values specified above.
if(NOT DEFINED TARGET_PLATFORM)
  set(TARGET_PLATFORM ${TARGET_STM32G0})
  message(
    STATUS
      "TARGET_PLATFORM not defined, using default value: ${TARGET_PLATFORM}")
endif()

if((NOT (${TARGET_PLATFORM} STREQUAL ${TARGET_STM32G0}))
   AND (NOT (${TARGET_PLATFORM} STREQUAL ${TARGET_NUCLEO_F1}))
   AND (NOT (${TARGET_PLATFORM} STREQUAL ${TARGET_DESKTOP})))
  message(NOTICE "Supported platfoms:")
  message(NOTICE "  Main target: ${TARGET_STM32G0}")
  message(NOTICE "  Testing platform (nucleo): ${TARGET_NUCLEO_F1}")
  message(NOTICE "  Desktop unit tests: ${TARGET_DESKTOP}")
  message(FATAL_ERROR "Platform ${TARGET_PLATFORM} is not supported")
endif()

set(TOOLCHAIN_FILE_DIR "${CMAKE_SOURCE_DIR}/cmake/ARMToolchain.cmake")

# Toolchain is used only during cross compilation
if(NOT ${BUILD_TEST})
  set(CMAKE_TOOLCHAIN_FILE ${TOOLCHAIN_FILE_DIR})
endif()

# Path to files generated with project configurator (CubeMX)
set(GENERATED_FILES_DIR "${CMAKE_SOURCE_DIR}/setup/WeathereverSTM")

# ##############################################################################
# FreeRTOS config
# ##############################################################################
if(UNIX)
  set(NATIVE_PLATFORM
      GCC_POSIX
      CACHE STRING "FreeRTOS port name")
else()
  set(NATIVE_PLATFORM
      MSVC_MINGW
      CACHE STRING "FreeRTOS port name")
endif()

if(${TARGET_PLATFORM} STREQUAL ${TARGET_STM32G0})
  set(FREERTOS_PORT "GCC_ARM_CM0")
elseif(${TARGET_PLATFORM} STREQUAL ${TARGET_NUCLEO_F1})
  set(FREERTOS_PORT "GCC_ARM_CM3")
elseif(${TARGET_PLATFORM} STREQUAL ${TARGET_DESKTOP})
  message(FATAL_ERROR "Native build is not supported")
  # set(FREERTOS_PORT ${NATIVE_PLATFORM})
endif()

# Only static allocation
set(FREERTOS_HEAP 0)
set(FREERTOS_DO_NOT_INCLUDE_HEAP ON)

# ##############################################################################
# Misc
# ##############################################################################
# C11 Support
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# C++17 Support
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json for editors which use them
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ##############################################################################
# General binary build options
# ##############################################################################
option(UPDATE_SUBMODULES
       "Check for submodules update before configuring project" OFF)
# Don't build unit tests by default
option(BUILD_TEST "Build unit test instead of cross compiling project" OFF)
