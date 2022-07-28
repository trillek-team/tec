cmake_minimum_required(VERSION 3.19)
project(trillek-client)

set(trillek-client_LIBRARY_NAME "client_lib")
set(trillek-client_LIBRARY_NAME ${trillek-client_LIBRARY_NAME} PARENT_SCOPE)
add_interface_lib(
	TARGET ${trillek-client_LIBRARY_NAME} 
	FILE_LIST
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
		resources/pixel-buffer.cpp)
link_client_libs(TARGET ${trillek-client_LIBRARY_NAME})

set(trillek-client_PROGRAM_NAME "trillek-client")
add_program(
	TARGET ${trillek-client_PROGRAM_NAME}
	FILE_LIST main.cpp
	LINK_LIBS PUBLIC ${trillek-client_LIBRARY_NAME})
