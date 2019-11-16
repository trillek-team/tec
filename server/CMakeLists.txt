# Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
# Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt
cmake_minimum_required(VERSION 3.9)
project(trillek-server)

include_directories(${trillek_SOURCE_DIR})
include_directories(SYSTEM ${BULLET_INCLUDE_DIR})
include_directories(SYSTEM ${trillek-proto-INCLUDE_DIR})
include_directories(SYSTEM ${trillek_SOURCE_DIR}/deps/selene/include)
include_directories(SYSTEM ${trillek_SOURCE_DIR}/deps/trillek-vcomputer/include)
include_directories(${trillek-server_SOURCE_DIR})

if (WIN32)
	set(trillek-server_PROGRAM "trillek-server")
else ()
	set(trillek-server_PROGRAM "trillek-serverd")
endif ()

set(trillek-server_SOURCES
	${trillek-server_SOURCE_DIR}/client-connection.cpp
	${trillek-server_SOURCE_DIR}/main.cpp
	${trillek-server_SOURCE_DIR}/server.cpp
)

add_executable(${trillek-server_PROGRAM} ${trillek-server_SOURCES})

if (MSVC)
	set_property(TARGET ${trillek-server_PROGRAM} APPEND_STRING PROPERTY COMPILE_FLAGS /W4)
else ()
	set_property(TARGET ${trillek-server_PROGRAM} APPEND_STRING PROPERTY COMPILE_FLAGS -Wall)
endif ()

target_link_libraries(${trillek-server_PROGRAM} ${trillek-common_LIBRARY} VCOMPUTER_STATIC)
