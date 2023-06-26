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

# Append path to cmake scripts directory
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# ##############################################################################
# Pre-project includes
# ##############################################################################
include(UpdateSubmodules)
include(SmallHelpers)
include(FetchContent)
# ##############################################################################

# ##############################################################################
# Add new options here
# ##############################################################################
option(UPDATE_SUBMODULES
       "Check for submodules update before configuring project" OFF)

# ##############################################################################

# Project versioning
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 1)

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

# Report used C and C++ standard
message(STATUS "Using C${CMAKE_C_STANDARD} standard")
message(STATUS "Using C++${CMAKE_CXX_STANDARD} standard")

# Make 10000% sure that git ignores binary directory
gen_gitignore(${CMAKE_BINARY_DIR}/../)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  set(CMAKE_TOOLCHAIN_FILE ARMToolchain)
endif()

# Check if project submodules are up to date
update_submodules(${UPDATE_SUBMODULES})

# ##############################################################################
# Pre-project configuration end
# ##############################################################################

project(
  Weatherever
  LANGUAGES C CXX
  DESCRIPTION "Remote weather station with anemometer"
)

# Make sure, that after reconfiguring, we've got
# fresh compile_commands.json in source directory
del_compile_commands(${CMAKE_SOURCE_DIR})

# Setup unit testing if applicable
fetch_test_fm(${BUILD_TEST})

# Move compile_commands.json to source dir for editors to use
place_comp_cmds_in_src_dir()
