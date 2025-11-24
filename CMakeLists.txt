cmake_minimum_required(VERSION 4.0.0)
project(RTSChess)

# ===== C++ Standard =====
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===== Compiler flags =====
set(COMPILE_FLAGS -Wall -Wextra -O2)

# ===== Shared =====
file(GLOB_RECURSE SHARED_SOURCES "${CMAKE_SOURCE_DIR}/shared/src/*.cpp")
set(SHARED_INCLUDE ${CMAKE_SOURCE_DIR}/shared/headers)

# ===== Client =====
file(GLOB_RECURSE CLIENT_SOURCES "${CMAKE_SOURCE_DIR}/client/src/*.cpp")
set(CLIENT_INCLUDE ${CMAKE_SOURCE_DIR}/client/headers)

add_executable(RTSChessClient ${CMAKE_SOURCE_DIR}/client/src/RTS-Chess-client.cpp ${SHARED_SOURCES} ${CLIENT_SOURCES})

target_include_directories(RTSChessClient PRIVATE ${SHARED_INCLUDE} ${CLIENT_INCLUDE})
target_compile_options(RTSChessClient PRIVATE ${COMPILE_FLAGS})
set_target_properties(RTSChessClient PROPERTIES OUTPUT_NAME "RTS-Chess-client")

# ===== Server =====
file(GLOB_RECURSE SERVER_SOURCES "${CMAKE_SOURCE_DIR}/server/src/*.cpp")
set(SERVER_INCLUDE ${CMAKE_SOURCE_DIR}/server/headers)

add_executable(RTSChessServer ${CMAKE_SOURCE_DIR}/server/src/RTS-Chess-server.cpp ${SHARED_SOURCES} ${SERVER_SOURCES})

target_include_directories(RTSChessServer PRIVATE ${SHARED_INCLUDE} ${SERVER_INCLUDE})
target_compile_options(RTSChessServer PRIVATE ${COMPILE_FLAGS})
set_target_properties(RTSChessServer PROPERTIES OUTPUT_NAME "RTS-Chess-server")
