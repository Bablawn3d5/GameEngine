#!/bin/bash -e

cmake -DCMAKE_INSTALL_PREFIX="`pwd`" -DCMAKE_BUILD_TYPE=Debug
make VERBOSE=1
ctest -C Debug
cpack
