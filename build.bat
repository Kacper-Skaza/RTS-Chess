@echo off
if exist build rmdir /s /q build
mkdir build

@echo on
cmake -S . -B build
cmake --build build --config Release
cmake --install build
