# Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
# Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt
cmake_minimum_required(VERSION 3.1)
project(trillek-client)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(OpenAL REQUIRED)
include_directories(${BULLET_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIR})
include_directories(${OPENAL_INCLUDE_DIR})
include_directories(${Protobuf_INCLUDE_DIR})
include_directories(${trillek_SOURCE_DIR})
include_directories(${trillek-client_SOURCE_DIR})
include_directories(${trillek_SOURCE_DIR}/deps/imgui)
include_directories(${trillek_SOURCE_DIR}/deps/spdlog/include)
include_directories(${trillek_SOURCE_DIR}/deps/selene/include)
include_directories(${trillek_SOURCE_DIR}/deps/trillek-vcomputer/include)
include_directories(${trillek-proto-INCLUDE_DIR})
add_definitions(-DASIO_STANDALONE)

if (WIN32)
	set(trillek-client_PROGRAM "trillek-client")
else ()
	set(trillek-client_PROGRAM "trillek")
endif ()
set(trillek-client_SOURCES
	${trillek_SOURCE_DIR}/deps/imgui/imgui.cpp
	${trillek_SOURCE_DIR}/deps/imgui/imgui_demo.cpp
	${trillek_SOURCE_DIR}/deps/imgui/imgui_draw.cpp
	${trillek-client_SOURCE_DIR}/imgui-system.cpp
	${trillek-client_SOURCE_DIR}/main.cpp
	${trillek-client_SOURCE_DIR}/os.cpp
	${trillek-client_SOURCE_DIR}/render-system.cpp
	${trillek-client_SOURCE_DIR}/server-connection.cpp
	${trillek-client_SOURCE_DIR}/sound-system.cpp
	${trillek-client_SOURCE_DIR}/test-data.cpp
	${trillek-client_SOURCE_DIR}/voxel-volume.cpp
	${trillek-client_SOURCE_DIR}/graphics/animation.cpp
	${trillek-client_SOURCE_DIR}/graphics/gbuffer.cpp
	${trillek-client_SOURCE_DIR}/graphics/material.cpp
	${trillek-client_SOURCE_DIR}/graphics/renderable.cpp
	${trillek-client_SOURCE_DIR}/graphics/shader.cpp
	${trillek-client_SOURCE_DIR}/graphics/texture-object.cpp
	${trillek-client_SOURCE_DIR}/graphics/vertex-buffer-object.cpp
	${trillek-client_SOURCE_DIR}/gui/console.cpp
	${trillek-client_SOURCE_DIR}/resources/md5anim.cpp
	${trillek-client_SOURCE_DIR}/resources/md5mesh.cpp
	${trillek-client_SOURCE_DIR}/resources/obj.cpp
	${trillek-client_SOURCE_DIR}/resources/vorbis-stream.cpp
	${trillek-client_SOURCE_DIR}/resources/pixel-buffer.cpp
)

# macOS needs to link libobjc
set(OSX_LIBRARIES "")
if (APPLE)
	set(OSX_LIBRARIES "/usr/lib/libobjc.dylib")
endif (APPLE)

add_executable(${trillek-client_PROGRAM} ${trillek-client_SOURCES})
if (MSVC)
	set_property(TARGET ${trillek-client_PROGRAM} APPEND_STRING PROPERTY COMPILE_FLAGS /W4)
else ()
	set_property(TARGET ${trillek-client_PROGRAM} APPEND_STRING PROPERTY COMPILE_FLAGS -Wall)
endif ()
target_link_libraries(${trillek-client_PROGRAM} ${trillek-common_LIBRARY}
	${BULLET_LIBRARIES} ${LUA_LIBRARIES} ${PROTOBUF_LIBRARIES} ${OPENGL_gl_LIBRARY}
	${GLFW_LIBRARY} ${OPENAL_LIBRARY} ${GLEW_LIBRARIES} ${OSX_LIBRARIES} VCOMPUTER_STATIC)
