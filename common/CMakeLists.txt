# Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
# Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt
cmake_minimum_required(VERSION 3.1)
project(trillek-common)

set(trillek-proto-INCLUDE_DIR ${trillek_BINARY_DIR}/common PARENT_SCOPE) # <-- Protobuf files go here

find_package(Bullet REQUIRED)
find_package(Lua REQUIRED)
find_package(Protobuf REQUIRED)
include_directories(${BULLET_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${PROTOBUF_INCLUDE_DIR})
include_directories(${trillek_BINARY_DIR}/common) # Protobuf files are here
include_directories(${trillek_SOURCE_DIR}/deps/selene/include)
include_directories(${trillek_SOURCE_DIR}/deps/spdlog/include)
include_directories(${trillek_SOURCE_DIR}/deps/trillek-vcomputer/include)
protobuf_generate_cpp(COMPONENTS_SOURCES COMPONENTS_HEADERS ${trillek-common_SOURCE_DIR}/proto/components.proto)
protobuf_generate_cpp(GAME_STATE_SOURCES GAME_STATE_HEADERS ${trillek-common_SOURCE_DIR}/proto/game_state.proto)

set(BUILD_STATIC_VCOMPUTER TRUE CACHE BOOL "Build Trillek VCOMPUTER library - static version")
set(BUILD_DYNAMIC_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER library - dynamic version")
set(BUILD_TOOLS_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER tools")
set(BUILD_TESTS_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER tests")
add_subdirectory(${trillek_SOURCE_DIR}/deps/trillek-vcomputer ${trillek_BINARY_DIR}/deps/trillek-vcomputer)

set(trillek-common_LIBRARY "tec" CACHE STRING "Name of the library with code common to both the client and the server")
set(trillek-common_SOURCES
	${trillek-common_SOURCE_DIR}/filesystem.cpp
	${trillek-common_SOURCE_DIR}/lua-system.cpp
	${trillek-common_SOURCE_DIR}/physics-system.cpp
	${trillek-common_SOURCE_DIR}/simulation.cpp
	${trillek-common_SOURCE_DIR}/string.cpp
	${trillek-common_SOURCE_DIR}/types.cpp
	${trillek-common_SOURCE_DIR}/vcomputer-system.cpp
	${trillek-common_SOURCE_DIR}/components/collision-body.cpp
	${trillek-common_SOURCE_DIR}/components/lua-script.cpp
	${trillek-common_SOURCE_DIR}/components/transforms.cpp
	${trillek-common_SOURCE_DIR}/controllers/fps-controller.cpp
	${trillek-common_SOURCE_DIR}/resources/mesh.cpp
	${trillek-common_SOURCE_DIR}/resources/script-file.cpp
)

add_library(${trillek-common_LIBRARY} STATIC ${COMPONENTS_SOURCES} ${GAME_STATE_SOURCES} ${trillek-common_SOURCES})
target_link_libraries(${trillek-common_LIBRARY} ${BULLET_LIBRARIES} ${LUA_LIBRARIES} ${PROTOBUF_LIBRARIES} VCOMPUTER_STATIC)
