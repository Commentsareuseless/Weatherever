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

# Those are the configuration defaults. Once the values are specified using
# -D... directive, it is being set in cache. The statements below just take
# effect once there is no entry of such variable in the cache (given by -D
# directive). So effectively, the values below are used only if not set "from
# outside"

# set(CMAKE_TARGET_PROCESSOR "x86_64" CACHE STRING "Target processor")
# message(STATUS "Target processor is ${CMAKE_TARGET_PROCESSOR}")

# Export compile_commands.json for editors which use them
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ##############################################################################
# Add new options here
# ##############################################################################
option(UPDATE_SUBMODULES
       "Check for submodules update before configuring project" OFF)

# ##############################################################################

# C99 Support
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Report used C standard
message(STATUS "Using C${CMAKE_C_STANDARD} standard")

# Check if project submodules are up to date
find_package(Git REQUIRED)
if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_LIST_DIR}/../.git")
  # Update submodules as needed
  message(STATUS "Found git, submodule update possible")
  if(UPDATE_SUBMODULES)
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

# Pre project includes
if(${CMAKE_BUILD_TYPE} STREQUAL "Test")
  # include(CTest)
else()
  include(ext/pico-sdk/pico_sdk_init.cmake)
endif()

# ##############################################################################
# Pre-project configuration end
# ##############################################################################

project(
  Weatherever
  LANGUAGES C
  DESCRIPTION "Remote weather station with anemometer")

# Include later used modules
if(${CMAKE_BUILD_TYPE} STREQUAL "Test")
  include(CTest)
  add_subdirectory(${CMAKE_SOURCE_DIR}/ext/unity)
  add_subdirectory(${CMAKE_SOURCE_DIR}/test)
else()
  pico_sdk_init()
endif()
