# Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
# Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt
cmake_minimum_required(VERSION 3.1)
project(trillek)

option(BUILD_CLIENT "Build the client" ON)
option(BUILD_SERVER "Build the server" ON)

# TODO: Perhaps add -pedantic -Wall -Wextra -Werror?
list(APPEND CMAKE_MODULE_PATH ${trillek_SOURCE_DIR}/deps)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${trillek_SOURCE_DIR}/common)

if (WIN32) # Windows
	# Library include directories for Lua and Protobuf
	set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH}
		${CMAKE_SOURCE_DIR}/lib/include/ ${trillek_SOURCE_DIR}/deps/protobuf/src)
	set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${CMAKE_SOURCE_DIR}/lib/)
	set(PROTOBUF_SRC_ROOT_FOLDER ${CMAKE_SOURCE_DIR}/lib/)
endif ()

add_subdirectory(common)
if (BUILD_CLIENT)
	add_subdirectory(client)
endif ()
if (BUILD_SERVER)
	add_subdirectory(server)
endif ()
