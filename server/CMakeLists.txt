cmake_minimum_required(VERSION 3.20)
project(trillek-server)

set(SERVER_LIB_NAME "server_lib")
set(SERVER_LIB_NAME ${SERVER_LIB_NAME} PARENT_SCOPE)

add_interface_lib(
	TARGET
	${SERVER_LIB_NAME}
	FILE_LIST
	client-connection.cpp
	lua-types.cpp
	save-game.cpp
	server.cpp
	server-game-state-queue.cpp
	user/user.cpp
)

# Set target name based on platform
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(SERVER_TARGET "trillekd")
else ()
	set(SERVER_TARGET "trillek-server")
endif ()

add_program(
	TARGET
	${SERVER_TARGET}
	FILE_LIST
	main.cpp
	LINK_LIBS
	PRIVATE
	${SERVER_LIB_NAME}
)
