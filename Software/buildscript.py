#!/usr/bin/python

# This script is meant to ease building
# multiple build configuration for multiple
# targets and switching betweent them.

import argparse
import os
import json

BUILDCACHE_FILE_NAME = "buildcache.json"
DEFAULT_BUILD_DIR: os.path = "build"
DEFAULT_UT_DIR: os.path = f"{DEFAULT_BUILD_DIR}{os.sep}native{os.sep}"
DEFAULT_TARGET_DIR: os.path = f"{DEFAULT_BUILD_DIR}{os.sep}target{os.sep}"
DEFAULT_BUILD_GENERATOR = "Ninja"
BUILD_TYPE_DBG = "Debug"
BUILD_TYPE_REL = "Release"

JSON_FILE_STRUCTURE = (
    '{"build_type" : "Debug, "generator" : "Ninja", "target" : "test"}'
)


TARGET_TEST = "test"
TARGET_INTEGRATION_TEST = "integration_test"
TARGET_DEV_STM_G03XX = "stmg03xx"


def parseCli():
    cliParser = argparse.ArgumentParser(
        description="buildscript - Build and manage build configurations"
    )
    cliParser.add_argument(
        "--target",
        action="store",
        dest="targetBuild",
        help="Target to build",
    )
    cliParser.add_argument(
        "-G",
        "--generator",
        action="store",
        dest="buildGenerator",
        help="CMake build generator (defaults to Ninja)",
    )
    cliParser.add_argument(
        "-p",
        "--program",
        action="store_true",
        dest="shouldProgramDevice",
        help="Flash program after build",
    )
    cliParser.add_argument(
        "--clean",
        action="store_true",
        dest="cleanBuild",
        help="Clear build artifacts",
    )
    cliParser.add_argument(
        "-t",
        "--type",
        action="store",
        dest="buildType",
        help="Type of build configuration (eg. Release/Debug)",
    )

    return cliParser.parse_args()


def tryReadJson(path: str):
    if os.path.exists(path):
        with open(path, mode="r", encoding="utf8") as cacheFile:
            cacheData = json.load(cacheFile)


def printErrInvTarget():
    print("[Err]\tInvalid target")
    print("[INF]\tSupported targets:")
    print(f"[INF]\t-{TARGET_DEV_STM_G03XX}")
    print(f"[INF]\t-{TARGET_TEST}")
    print(f"[INF]\t-{TARGET_INTEGRATION_TEST}")


def main():
    args = parseCli()

    if not os.path.exists(f"./{DEFAULT_BUILD_DIR}/"):
        os.mkdir(f"./{DEFAULT_BUILD_DIR}/")

    # Read json cache from build/buildcache.json
    # if cache is available
    if os.path.exists(f"./{DEFAULT_BUILD_DIR}/{BUILDCACHE_FILE_NAME}"):
        pass

    # If there is no cache, create one and insert there
    # default options

    # Parse input arguments
    if TARGET_DEV_STM_G03XX == args.targetBuild:
        pass
    elif TARGET_TEST == args.targetBuild:
        pass
    elif TARGET_INTEGRATION_TEST == args.targetBuild:
        pass
    else:
        printErrInvTarget()

    # Depending on cache or cli args compose apropiate
    # build configuration directory
    # i.e. build/target/Debug, build/target/Release

    # Invoke cmake with arguments from cli or cache
    # If apropriate projec configuration exist, skip configure step
    # Otherwise first time configure project with cmake


if __name__ == "__main__":
    main()
