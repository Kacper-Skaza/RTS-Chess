#!/bin/bash
set -e

[ -d build ] && rm -rf build
mkdir build

cd build

cmake ..
cmake --build . --config Release

cd ..
