@echo off
if exist build rmdir /s /q build
mkdir build

cd build
@echo on

cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
cmake  --build .

@echo off
cd ..
