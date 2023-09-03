# Helper macro for adding new libs to avoid copy-pasting the same code

macro(add_static_lib)

  # Tag arguments for CMake parser
  set(option_args OPTIONS)
  set(one_val_args LIB_NAME LIB_NAMESPACE)
  set(multi_val_args LIB_SRC LINK_LIBS LINK_DIRS)

  cmake_parse_arguments(ARG "${option_args}" "${one_val_args}"
                        "${multi_val_args}" ${ARGN})

  add_library(${ARG_LIB_NAME} STATIC "${ARG_LIB_SRC}")
  target_link_libraries(${ARG_LIB_NAME} PUBLIC ${ARG_LINK_LIBS})
  target_compile_options(${ARG_LIB_NAME} PRIVATE -Wall -Wextra -Wpedantic)
  target_include_directories(${ARG_LIB_NAME} PRIVATE ${ARG_LINK_DIRS})

  add_library(${ARG_LIB_NAMESPACE}::${ARG_LIB_NAME} ALIAS ${ARG_LIB_NAME})
endmacro(add_static_lib)
