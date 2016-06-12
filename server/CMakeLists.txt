# Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
# Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt
cmake_minimum_required(VERSION 3.1)
project(trillek-server)

include_directories(${BULLET_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${Protobuf_INCLUDE_DIR})
include_directories(${trillek_SOURCE_DIR})
include_directories(${trillek_SOURCE_DIR}/deps/spdlog/include)
include_directories(${trillek_SOURCE_DIR}/deps/trillek-vcomputer/include)
include_directories(${trillek-proto-INCLUDE_DIR})

add_definitions(-DASIO_STANDALONE)

if (WIN32)
	set(trillek-server_PROGRAM "trillek-server")
else ()
	set(trillek-server_PROGRAM "trillekd")
endif ()
set(trillek-server_SOURCES
	${trillek-server_SOURCE_DIR}/client-connection.cpp
	${trillek-server_SOURCE_DIR}/main.cpp
	${trillek-server_SOURCE_DIR}/server.cpp
)

add_executable(${trillek-server_PROGRAM} ${trillek-server_SOURCES})
target_link_libraries(${trillek-server_PROGRAM} ${trillek-common_LIBRARY}
	${BULLET_LIBRARIES} ${LUA_LIBRARIES} ${PROTOBUF_LIBRARIES} VCOMPUTER_STATIC)
