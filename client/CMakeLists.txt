cmake_minimum_required(VERSION 3.20)
project(trillek-client)

set(CLIENT_LIB_NAME "client_lib")
set(CLIENT_LIB_NAME ${CLIENT_LIB_NAME} PARENT_SCOPE)

add_interface_lib(
	TARGET
	${CLIENT_LIB_NAME}
	FILE_LIST
	application.cpp
	game.cpp
	imgui-system.cpp
	client-game-state-queue.cpp
	os.cpp
	lua-types.cpp
	render-system.cpp
	server-connection.cpp
	sound-system.cpp
	test-data.cpp
	voxel-volume.cpp
	graphics/animation.cpp
	graphics/gbuffer.cpp
	graphics/material.cpp
	graphics/renderable.cpp
	graphics/shader.cpp
	graphics/texture-object.cpp
	graphics/vertex-buffer-object.cpp
	gui/console.cpp
	gui/active-entity-tooltip.cpp
	gui/server-connect.cpp
	gui/debug-info.cpp
	manipulators/placement.cpp
	resources/md5anim.cpp
	resources/md5mesh.cpp
	resources/obj.cpp
	resources/vorbis-stream.cpp
	resources/pixel-buffer.cpp
)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

target_link_libraries(${CLIENT_LIB_NAME} INTERFACE glad::glad glfw OpenAL::OpenAL imgui::imgui)

link_opengl_libs(TARGET ${CLIENT_LIB_NAME})

add_program(
	TARGET
	trillek-client
	FILE_LIST
	main.cpp
	LINK_LIBS
	PUBLIC
	${CLIENT_LIB_NAME}
)
