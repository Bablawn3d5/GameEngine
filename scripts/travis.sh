#!/bin/bash -e

cmake -DCMAKE_BUILD_TYPE=Debug -DENTITYX_BUILD_TESTING=0
make VERBOSE=1
