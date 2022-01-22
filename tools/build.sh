#!/bin/bash

mkdir build
cd build
set -x

if [ -z ${1+x} ]; then
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=1 -DBUILD_EXAMPLES=1 ..;
else
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 -DBUILD_TESTS=1 -DBUILD_EXAMPLES=1 ..;
fi

set +x
make -j$(nproc)