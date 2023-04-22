if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
  message(
    FATAL_ERROR
      "Prevented in-tree build. Please create a build directory outside of current project and run cmake"
  )
endif()

cmake_minimum_required(VERSION 3.21)

# ##############################################################################
# Pre-project configuration
# ##############################################################################

# Export compile_commands.json for editors which use them
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ##############################################################################
# Add new options here
# ##############################################################################
option(UPDATE_SUBMODULES
       "Check for submodules update before configuring project" OFF)

# ##############################################################################

# Convenience, not to copy-paste this in different generated dirs
function(gen_gitignore DIR)
  file(WRITE ${DIR}/.gitignore "# Ignore everything inside this directory\n")
  file(APPEND ${DIR}/.gitignore "./*")
endfunction(gen_gitignore DIR)


# C11 Support
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# C++17 Support
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Report used C and C++ standard
message(STATUS "Using C${CMAKE_C_STANDARD} standard")
message(STATUS "Using C++${CMAKE_CXX_STANDARD} standard")

if(NOT (${CMAKE_BUILD_TYPE} STREQUAL "Test"))
  # Make sure this script will work on Windows
  if(WIN32)
    set(TOOLCHAIN_EXT ".exe")
  else()
    set(TOOLCHAIN_EXT "")
  endif(WIN32)

  # Set common variables for cross compilation
  set(TARGET_TRIPLET "arm-none-eabi-")
  set(CMAKE_SYSTEM_NAME Generic)
  set(CMAKE_SYSTEM_PROCESSOR arm)
  set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

  # Use apropriate compilers and utilis
  set(CMAKE_C_COMPILER   ${TARGET_TRIPLET}gcc${TOOLCHAIN_EXT})
  set(CMAKE_CXX_COMPILER ${TARGET_TRIPLET}g++${TOOLCHAIN_EXT})
  set(CMAKE_ASM_COMPILER ${TARGET_TRIPLET}gcc${TOOLCHAIN_EXT})
  set(CMAKE_LINKER       ${TARGET_TRIPLET}gcc${TOOLCHAIN_EXT})
  set(CMAKE_SIZE_UTIL    ${TARGET_TRIPLET}size${TOOLCHAIN_EXT})
  set(CMAKE_OBJCOPY      ${TARGET_TRIPLET}objcopy${TOOLCHAIN_EXT})
  set(CMAKE_OBJDUMP      ${TARGET_TRIPLET}objdump${TOOLCHAIN_EXT})
  set(CMAKE_NM_UTIL      ${TARGET_TRIPLET}gcc-nm${TOOLCHAIN_EXT})
  set(CMAKE_AR           ${TARGET_TRIPLET}gcc-ar${TOOLCHAIN_EXT})
  set(CMAKE_RANLIB       ${TARGET_TRIPLET}gcc-ranlib${TOOLCHAIN_EXT})

  # Define target processor
  set(MCU_TYPE "-mcpu=cortex-m0plus")
  set(MCPU_FLAGS "-mthumb ${MCU_TYPE}")

  # Compiler and linker flags
  set(LDSCRIPT STM32G030C8Tx_FLASH.ld)
  set(LIBS -lc -lm -lnosys)
  set(LD_FLAGS ${MCU_TYPE} -specs=nano.specs -T${LDSCRIPT} ${LIBS} -Wl,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map,--cref -Wl,--gc-sections)
  set(CMAKE_COMMON_FLAGS -ffunction-sections -fdata-sections -fno-builtin -fno-common -Wall -Wshadow -Wdouble-promotion -Werror)

  set(CMAKE_C_FLAGS ${MCPU_FLAGS} ${CMAKE_COMMON_FLAGS})
  set(CMAKE_CXX_FLAGS ${MCPU_FLAGS} ${CMAKE_COMMON_FLAGS})
  set(CMAKE_ASM_FLAGS ${MCPU_FLAGS} ${CMAKE_COMMON_FLAGS})
  set(CMAKE_EXE_LINKER_FLAGS ${LD_FLAGS},-print-memory-usage)

  set(CMAKE_C_FLAGS_DEBUG "-O0 -g -gdwarf-2")
  set(CMAKE_CXX_ASM_FLAGS_DEBUG "-O0 -g -gdwarf-2")
  set(CMAKE_ASM_FLAGS_DEBUG "")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "")

  set(CMAKE_C_FLAGS_RELEASE "-Os -flto")
  set(CMAKE_CXX_FLAGS_RELEASE "-Os -flto")
  set(CMAKE_ASM_FLAGS_RELEASE "")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")

  # Move configuration from MX Cube to main project directory
  set(PRJ_SETUP_DIR ${CMAKE_SOURCE_DIR}/setup/WeathereverSTM)
  # Base generated project files
  file(INSTALL ${PRJ_SETUP_DIR}/Core/Inc DESTINATION ${CMAKE_SOURCE_DIR}/core/gen )
  file(INSTALL ${PRJ_SETUP_DIR}/Core/Src DESTINATION ${CMAKE_SOURCE_DIR}/core/gen )
  # Core drivers for MCU
  file(INSTALL ${PRJ_SETUP_DIR}/Drivers/ DESTINATION ${CMAKE_SOURCE_DIR}/cdrv )
  # FreeRTOS files
  file(INSTALL ${PRJ_SETUP_DIR}/Middlewares/Third_Party/FreeRTOS/Source/ DESTINATION ${CMAKE_SOURCE_DIR}/rtos )
  # Linker script for STM32G030C8T
  file(INSTALL ${PRJ_SETUP_DIR}/STM32G030C8Tx_FLASH.ld DESTINATION ${CMAKE_SOURCE_DIR} )
  # Microcontroler startup code
  file(INSTALL ${PRJ_SETUP_DIR}/startup_stm32g030xx.s DESTINATION ${CMAKE_SOURCE_DIR} )
  # Since there quite a lot of files in setup dir, we don't want to make unneccessary copies
  # Leave only CubeMX project files
#   file(REMOVE_RECURSE ${PRJ_SETUP_DIR}/Core ${PRJ_SETUP_DIR}/Drivers ${PRJ_SETUP_DIR}/Middlewares)
#   file(REMOVE
#     ${PRJ_SETUP_DIR}/startup_stm32g030xx.s
#     ${PRJ_SETUP_DIR}/STM32G030C8Tx_FLASH.ld
#     ${PRJ_SETUP_DIR}/STM32G030K8Tx_FLASH.ld
#     ${PRJ_SETUP_DIR}/Makefile
#   )
endif()

# Check if project submodules are up to date
find_package(Git REQUIRED)
if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_LIST_DIR}/../.git")
  # Update submodules as needed
  message(STATUS "Found git, submodule update possible")
  if(UPDATE_SUBMODULES EQUAL ON)
    message(STATUS "Submodule update")
    execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
      WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/
      RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
      message(
        FATAL_ERROR
          "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules"
      )
    endif()
  endif()
endif()

# Make 10000% sure that git ignores binary directory
file(WRITE ${CMAKE_BINARY_DIR}/../.gitignore "# Ignore everything inside this directory\n")
file(APPEND ${CMAKE_BINARY_DIR}/../.gitignore "./*")

# ##############################################################################
# Pre-project configuration end
# ##############################################################################

project(
  Weatherever
  LANGUAGES C CXX
  DESCRIPTION "Remote weather station with anemometer"
)

# Include later used modules
if(${CMAKE_BUILD_TYPE} STREQUAL "Test")
  include(CTest)
  include(FetchContent)

  # CppUTest
  set(TEST_FRAMEWORK_DIR ${CMAKE_SOURCE_DIR}/test/CppUTest)

  FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG        master
  )
  FetchContent_MakeAvailable(CppUTest)

  add_subdirectory(${CMAKE_SOURCE_DIR}/test)

  else()
  #   pico_sdk_init()
endif()

# Move compile_commands.json to main directory for editors which use them
add_custom_command(
  OUTPUT ${CMAKE_SOURCE_DIR}/compile_commands.json
  POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_BINARY_DIR}/compile_commands.json
  ${CMAKE_SOURCE_DIR}/
)

add_custom_target(
  CompileCmds ALL
  DEPENDS ${CMAKE_SOURCE_DIR}/compile_commands.json
)
