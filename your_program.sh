#!/bin/sh
#
# Use this script to run your program LOCALLY.
#
# Note: Changing this script WILL NOT affect how CodeCrafters runs your program.
#
# Learn more: https://codecrafters.io/program-interface

set -e # Exit early if any commands fail
export OPENROUTER_API_KEY="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"
export VCPKG_ROOT=$HOME/vcpkg
export VCPKG_FORCE_SYSTEM_BINARIES=1

# Copied from .codecrafters/compile.sh
#
# - Edit this to change how your program compiles locally
# - Edit .codecrafters/compile.sh to change how your program compiles remotely
(
  cd "$(dirname "$0")" # Ensure compile steps are run within the repository directory
  # cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
  # cmake --build ./build
  if [ ! -f build/claude-code ]; then
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
    cmake --build ./build
  fi
)

# Copied from .codecrafters/run.sh
#
# - Edit this to change how your program runs locally
# - Edit .codecrafters/run.sh to change how your program runs remotely
exec $(dirname "$0")/build/claude-code "$@"
