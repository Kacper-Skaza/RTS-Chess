@echo off
if exist build rmdir /s /q build
mkdir build

@echo on
@REM cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug
cmake -S . -B build -G "Ninja" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug

cmake --build build
cmake --install build
