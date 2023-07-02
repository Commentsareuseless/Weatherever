# CMake toolchain for ARM development
# (Bare metal or RTOS configuration)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

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
# set(CMAKE_COMMON_FLAGS -ffunction-sections -fdata-sections -fno-builtin -fno-common -Wall -Wshadow -Wdouble-promotion)

set(CMAKE_C_FLAGS ${MCPU_FLAGS})# ${CMAKE_COMMON_FLAGS})
set(CMAKE_CXX_FLAGS ${MCPU_FLAGS})# ${CMAKE_COMMON_FLAGS})
set(CMAKE_ASM_FLAGS ${MCPU_FLAGS})# ${CMAKE_COMMON_FLAGS})

set(CMAKE_C_FLAGS_DEBUG "-O0 -g -gdwarf-2")
set(CMAKE_CXX_ASM_FLAGS_DEBUG "-O0 -g -gdwarf-2")
set(CMAKE_ASM_FLAGS_DEBUG "")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "")

set(CMAKE_C_FLAGS_RELEASE "-Os -flto")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -flto")
set(CMAKE_ASM_FLAGS_RELEASE "")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
