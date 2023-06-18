# CMake toolchain for ARM development
# (Bare metal or RTOS configuration)

macro(toolchainSetup)

  # Make sure this script will work on Windows
  set(TOOLCHAIN_EXT ${CMAKE_EXECUTABLE_SUFFIX})

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

endmacro(toolchainSetup)
