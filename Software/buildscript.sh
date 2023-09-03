#!/bin/bash
(
set -e   # Stop execution when error occurs

BUILD_DIR='build'
BUILD_NATIVE=$BUILD_DIR/native  # The Debug and test config
BUILD_TARGET=$BUILD_DIR/target  # The Release and HW test config
BUILD_TYPE_DBG="Debug"
BUILD_TYPE_REL="Release"
BUILD_TYPE_SEL="$BUILD_TYPE_DBG"
BUILD_GEN_POSTFIX= #" Makefiles"
BUILD_GENERATOR="Ninja"$BUILD_GEN_POSTFIX

################################################################################
# Helper functions
################################################################################

only_build_project() {
  cmake --build $BUILD_TARGET --config $BUILD_TYPE_SEL -j `nproc`
}

build_and_cfg_project() {
  cmake -G "$BUILD_GENERATOR" -B $BUILD_TARGET -S . -DCMAKE_BUILD_TYPE=$BUILD_TYPE_SEL -DUPDATE_SUBMODULES=ON -DBUILD_TEST=OFF
  only_build_project
}

build_test() {
  cmake --build $BUILD_NATIVE --config $BUILD_TYPE_SEL -j `nproc`
}

cfg_and_build_test() {
  cmake -G "$BUILD_GENERATOR" -B $BUILD_NATIVE -S . -DCMAKE_BUILD_TYPE=$BUILD_TYPE_SEL -DUPDATE_SUBMODULES=ON -DBUILD_TEST=ON
  build_test
}

run_tests() {
  ctest --test-dir $BUILD_NATIVE
}

cleanup() {
  if [ -d $BUILD_TARGET ]; then
    cmake --build $BUILD_TARGET --target clean
  elif [ -d $BUILD_NATIVE ]; then
    cmake --build $BUILD_NATIVE --target clean
  fi
}

################################################################################
# Handle user input
################################################################################

if [ $# == 0 ]; then
  # If no arguments passed, just build tests
  BUILD_TYPE_SEL=$BUILD_TYPE_DBG
  build_test

elif [ $1 == "clean" ]; then
  cleanup

elif [ $1 == "cfg" ]; then
  # Build with configure
  BUILD_TYPE_SEL=$BUILD_TYPE_REL
  build_and_cfg_project

elif [ $1 == "ncfg" ]; then
  # Build without config step
  only_build_project

elif [ $1 == "test-cfg" ]; then
  BUILD_TYPE_SEL=$BUILD_TYPE_DBG
  cfg_and_build_test

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
    echo "ncfg          - Just build project for target"
    echo "clean         - Clean build directories"
    echo "prg           - Inactive, WIP" # Build for target and program device
fi
)