cmake_minimum_required(VERSION 3.19)

enable_testing()
include(Dart)
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

set(trillek-test_PROGRAM_NAME "tests")
add_program(
	TARGET ${trillek-test_PROGRAM_NAME}
	FILE_LIST
		filesystem_test.cpp
		net-message_test.cpp
		save-game_test.cpp
		server-client-connection.cpp
		user_test.cpp
	LINK_LIBS PRIVATE
		GTest::gtest GTest::gtest_main
		${trillek-client_LIBRARY_NAME}
		${trillek-server_LIBRARY_NAME})

target_include_directories(${trillek-test_PROGRAM_NAME} PUBLIC ${CMAKE_HOME_DIRECTORY})

gtest_discover_tests(${trillek-test_PROGRAM_NAME} WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY})
