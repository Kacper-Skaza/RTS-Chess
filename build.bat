@echo off
if exist build rmdir /s /q build
mkdir build

cd build
@echo on

cmake ..
cmake --build . --config Release

@echo off
cd ..
