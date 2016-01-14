#!/bin/bash -e

cmake -DCMAKE_INSTALL_PREFIX="`pwd`" -DCMAKE_BUILD_TYPE=Debug -DENTITYX_BUILD_TESTING=0 -DBOOST_ROOT=$BOOST_ROOT
make VERBOSE=1
make test || (cat Testing/Temporary/LastTest.log && exit 1)
cpack
