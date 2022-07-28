cmake_minimum_required(VERSION 3.19)
project(trillek-server)

set(trillek-server_LIBRARY_NAME "server_lib")
set(trillek-server_LIBRARY_NAME ${trillek-server_LIBRARY_NAME} PARENT_SCOPE)
add_interface_lib(
	TARGET ${trillek-server_LIBRARY_NAME}
	FILE_LIST
		client-connection.cpp
		lua-types.cpp
		save-game.cpp
		server.cpp
		server-game-state-queue.cpp
		user/user.cpp)

if (WIN32)
	set(trillek-server_PROGRAM_NAME "trillek-server")
else ()
	set(trillek-server_PROGRAM_NAME "trillek-serverd")
endif ()
add_program(
	TARGET ${trillek-server_PROGRAM_NAME}
	FILE_LIST main.cpp
	LINK_LIBS PUBLIC ${trillek-server_LIBRARY_NAME})

