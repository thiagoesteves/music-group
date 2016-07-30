#!/bin/bash

TARGET_RELEASE="debug"
TARGET_TEST="debug-test"

function log_error {
  echo " Invalid target "
  echo ""
  echo " Usage: "
  echo " ./clean <target>"
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

# Clean the target
BUILD_DIR="build/$1"
rm -rf $BUILD_DIR

