#!/bin/bash

BUILD_DIR='build'
BUILD_NATIVE=$BUILD_DIR/native
BUILD_TARGET=$BUILD_DIR/target
BUILD_TYPE_DBG="Debug"
BUILD_TYPE_REL="Release"
BUILD_TYPE_TEST="Test"
BUILD_TYPE_SEL="$BUILD_TYPE_TEST"
BUILD_GENERATOR="Ninja"

################################################################################
# Helper functions
################################################################################

# create_gitignore() {
#   # Create .gitignore inside build directory
#   echo '# Ignore everything inside this directory' > $BUILD_DIR/.gitignore
#   echo './*' >> $BUILD_DIR/.gitignore
# }

only_build_project() {
  cmake --build $BUILD_TARGET --config $BUILD_TYPE_SEL -j `nproc`
}

build_and_cfg_project() {
  cmake -G $BUILD_GENERATOR -B $BUILD_TARGET -S . -DCMAKE_BUILD_TYPE=$BUILD_TYPE_SEL -DUPDATE_SUBMODULES=ON
  only_build_project
}

build_test() {
  cmake --build $BUILD_NATIVE --config $BUILD_TYPE_SEL -j `nproc`
}

cfg_and_build_test() {
  cmake -G $BUILD_GENERATOR -B $BUILD_NATIVE -S . -DCMAKE_BUILD_TYPE=$BUILD_TYPE_SEL -DUPDATE_SUBMODULES=ON
  build_test 
}

run_tests() {
  ctest --test-dir $BUILD_NATIVE
}

cleanup() {
  cmake --build $BUILD_NATIVE --target clean
  cmake --build $BUILD_TARGET --target clean
}

move_compile_cmds() {
  mv -f $1/compile_commands.json .
}

################################################################################
# Handle user input
################################################################################

if [ $# == 0 ]; then
  # If no arguments passed, just build tests
  BUILD_TYPE_SEL=$BUILD_TYPE_TEST
  build_test

elif [ $1 == "clean" ]; then
  cleanup

elif [ $1 == "cfg" ]; then
  # Build with configure
  BUILD_TYPE_SEL=$BUILD_TYPE_DBG
  build_and_cfg_project
  move_compile_cmds $BUILD_TARGET

elif [ $1 == "ncfg" ]; then
  # Build without config step
  only_build_project

elif [ $1 == "test-cfg" ]; then
  BUILD_TYPE_SEL=$BUILD_TYPE_TEST
  cfg_and_build_test
  move_compile_cmds $BUILD_NATIVE

elif [ $1 == "test-run" ]; then
  run_tests
else
# Print usage
    echo "Usage: "
    echo "  ./buildscript.sh [options]"
    echo ""
    echo "Available options:"
    echo "<no-option>   - Build tests"
    echo "test-cfg      - Configure and build tests"
    echo "test-run      - Only run tests"
    echo "cfg           - First time configure and build"
fi
