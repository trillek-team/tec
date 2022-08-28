cmake_minimum_required(VERSION 3.19)
project(trillek-server)

set(SERVER_LIB_NAME "server_lib")
set(SERVER_LIB_NAME ${SERVER_LIB_NAME} PARENT_SCOPE)
add_interface_lib(
	TARGET ${SERVER_LIB_NAME}
	FILE_LIST
		client-connection.cpp
		lua-types.cpp
		save-game.cpp
		server.cpp
		server-game-state-queue.cpp
		user/user.cpp)

if (WIN32)
	set(SERVER_NAME "trillek-server")
else ()
	set(SERVER_NAME "trillek-serverd")
endif ()
add_program(TARGET ${SERVER_NAME} FILE_LIST main.cpp LINK_LIBS PUBLIC ${SERVER_LIB_NAME})
