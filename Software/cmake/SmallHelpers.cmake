# This file contains small (max 10 lines long) convenience functions

function(gen_gitignore DIR)
  file(WRITE ${DIR}/.gitignore "# Ignore everything inside this directory\n")
  file(APPEND ${DIR}/.gitignore "./*")
endfunction(gen_gitignore DIR)

# This function is called to trigger compile_commands.json 
# regeneration when reconfiguring is required
function(del_compile_commands CM_COM_DIR)
  file(REMOVE ${CM_COM_DIR}/compile_commands.json)
endfunction()

macro(place_comp_cmds_in_src_dir)
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
endmacro(place_comp_cmds_in_src_dir)

macro(fetch_test_fm SHOULD_BUILD_UT)
  if(${SHOULD_BUILD_UT})
    message(STATUS "BUILD_TEST is ${BUILD_TEST}, UT will be built")
    include(CTest)

    # GTest
    set(TEST_FRAMEWORK_DIR ${CMAKE_SOURCE_DIR}/test/)

    FetchContent_Declare(
      GoogleTest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG        v1.13.0
    )
    FetchContent_MakeAvailable(GoogleTest)

    add_subdirectory(${CMAKE_SOURCE_DIR}/test)
  endif()
endmacro(fetch_test_fm)

