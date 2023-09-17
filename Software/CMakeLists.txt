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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ##############################################################################
# Pre-project includes
# ##############################################################################
include(UpdateSubmodules)
include(SmallHelpers)
include(FetchContent)
include(ConfigVariables)
# ##############################################################################

# Report used C and C++ standard
message(STATUS "Using C${CMAKE_C_STANDARD} standard")
message(STATUS "Using C++${CMAKE_CXX_STANDARD} standard")

# Make 10000% sure that git ignores binary directory
gen_gitignore(${CMAKE_BINARY_DIR}/../)

# Check if project submodules are up to date
update_submodules(${UPDATE_SUBMODULES})

# ##############################################################################
# Pre-project configuration end
# ##############################################################################

project(
  Weatherever
  LANGUAGES C CXX ASM
  DESCRIPTION "Remote weather station with anemometer")

add_subdirectory(ext/etl)

if(NOT ${BUILD_TEST})
  # Project modules
  add_subdirectory(setup)
  add_subdirectory(include)
  add_subdirectory(rtos)
  add_subdirectory(core)
  add_subdirectory(drv)
else()
  add_subdirectory(ext/fff)
endif()

# Make sure, that after reconfiguring, we've got fresh compile_commands.json in
# source directory
del_compile_commands(${CMAKE_SOURCE_DIR})

# Setup unit testing if applicable
fetch_test_fm(${BUILD_TEST})

# Move compile_commands.json to source dir for editors to use
place_comp_cmds_in_src_dir()
