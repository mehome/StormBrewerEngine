#!/bin/bash
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_CLIENT=OFF $1

