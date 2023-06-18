# This script checks if all submodules are up to date
function(update_submodules)
  # Parse parameters
  set(option_args SHOULD_UPDATE)
  set(one_val_args)
  set(multi_val_args)
  cmake_parse_arguments(ARG "${option_args}" "${one_val_args}"
                            "${multi_val_args}" ${ARGN})

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
endfunction()
