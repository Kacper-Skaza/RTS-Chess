#!/bin/bash
set -e
[ -d build ] && rm -rf build
mkdir build

cmake -S . -B build
cmake --build build --config Release
cmake --install build
