#!/bin/bash

TARGET_RELEASE="debug"
TARGET_TEST="debug-test"

function log_error {
  echo " Invalid target "
  echo ""
  echo " Usage: "
  echo " ./build <target>"
  echo ""
  echo "Parameters:"
  echo "target: $TARGET_RELEASE or $TARGET_TEST"
  exit
}

# check parameters
if [ $TARGET_RELEASE !=  "$1" ] && [ $TARGET_TEST !=  "$1" ] ; then
  log_error
fi

# Absolute path to this script
SCRIPT=$(readlink -f $0)
# Absolute path this script is in
SCRIPTPATH=`dirname $SCRIPT`

# Go to the root of the project
cd $SCRIPTPATH/..

# Define build directory
BUILD_DIR="build/$1"
cmake -E make_directory $BUILD_DIR
cd $BUILD_DIR

if [ $TARGET_TEST ==  "$1" ] ; then
  cmake -Dtest=ON ../..
else
  cmake -Dtest=OFF ../..
fi

# Build the project
make

