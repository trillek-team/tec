cmake_minimum_required(VERSION 3.20)

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

set(trillek-test_PROGRAM_NAME "tests")
add_program(
	TARGET
	${trillek-test_PROGRAM_NAME}
	FILE_LIST
	filesystem_test.cpp
	net-message_test.cpp
	save-game_test.cpp
	server-client-connection.cpp
	user_test.cpp
	LINK_LIBS
	PRIVATE
	GTest::gtest
	GTest::gtest_main
	${CLIENT_LIB_NAME}
	${SERVER_LIB_NAME}
)

target_include_directories(${trillek-test_PROGRAM_NAME} PRIVATE ${CMAKE_HOME_DIRECTORY})

gtest_discover_tests(${trillek-test_PROGRAM_NAME} WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY})
