cmake_minimum_required(VERSION 3.21.0)
project(RTSChess)

# ===== C++ Standard =====
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===== Compile options =====
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_compile_options(-fdiagnostics-color=always)
endif()

# ===== Find SDL2 dependencies =====
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

# ===== Compiler and linker flags =====
set(COMPILE_FLAGS -Wall -Wextra -O2)
set(LINK_FLAGS_CLIENT SDL2 SDL2_ttf SDL2_image)
set(LINK_FLAGS_SHARED "")

# ===== Platform dependent flags =====
if(WIN32 OR WIN64)
	list(APPEND LINK_FLAGS_SHARED ws2_32)
endif()

# ===== Shared =====
file(GLOB_RECURSE SHARED_SOURCES "${CMAKE_SOURCE_DIR}/shared/src/*.cpp")
set(SHARED_INCLUDE ${CMAKE_SOURCE_DIR}/shared/headers)

# ===== Client =====
file(GLOB_RECURSE CLIENT_SOURCES "${CMAKE_SOURCE_DIR}/client/src/*.cpp")
set(CLIENT_INCLUDE ${CMAKE_SOURCE_DIR}/client/headers)

add_executable(
	RTSChessClient
	${CMAKE_SOURCE_DIR}/client/src/RTS-Chess-client.cpp
	${SHARED_SOURCES}
	${CLIENT_SOURCES}
)

target_include_directories(RTSChessClient PRIVATE ${SDL2_INCLUDE_DIRS} ${SHARED_INCLUDE} ${CLIENT_INCLUDE})
target_compile_options(RTSChessClient PRIVATE ${COMPILE_FLAGS})
target_link_libraries(RTSChessClient PRIVATE ${LINK_FLAGS_SHARED} ${LINK_FLAGS_CLIENT})
set_target_properties(RTSChessClient PROPERTIES OUTPUT_NAME "RTS-Chess-client")

# ===== Server =====
file(GLOB_RECURSE SERVER_SOURCES "${CMAKE_SOURCE_DIR}/server/src/*.cpp")
set(SERVER_INCLUDE ${CMAKE_SOURCE_DIR}/server/headers)

add_executable(
	RTSChessServer
	${CMAKE_SOURCE_DIR}/server/src/RTS-Chess-server.cpp
	${SHARED_SOURCES}
	${SERVER_SOURCES}
)

target_include_directories(RTSChessServer PRIVATE ${SHARED_INCLUDE} ${SERVER_INCLUDE})
target_compile_options(RTSChessServer PRIVATE ${COMPILE_FLAGS})
target_link_libraries(RTSChessServer PRIVATE ${LINK_FLAGS_SHARED})
set_target_properties(RTSChessServer PROPERTIES OUTPUT_NAME "RTS-Chess-server")

# ===== Install configuration =====
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
install(TARGETS RTSChessClient RUNTIME DESTINATION release)
install(TARGETS RTSChessServer RUNTIME DESTINATION release)
