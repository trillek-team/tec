# Provide a local project and cmake_minimum_required calls
# to permit to generate the documentation without configuring
# the complete project

# This variable should contain the argument of the project call in the main CMakeLists.txt
set(ORIGINAL_PROJECT_NAME "Project Trillek")
set(ORIGINAL_PROJECT_VERSION "${PROJECT_VERSION}")

project("${ORIGINAL_PROJECT_NAME} Docs")

include(GNUInstallDirs)

find_package(Doxygen QUIET)

if(DOXYGEN_FOUND)
	set(DOXYGEN_BUILD_DIR "doxygen" CACHE STRING "Build directory for Doxygen html output (absolute).")

	# This variables are used inside the Doxyfile.in template
	set(DOXYGEN_PROJECT_BRIEF "0x10c Inspired space game with custom engine")
	set(DOXYGEN_COMMON_SOURCE_DIR "common")
	set(DOXYGEN_CLIENT_SOURCE_DIR "client")
	set(DOXYGEN_SERVER_SOURCE_DIR "server")
	set(DOXYGEN_EXCLUDE_DIR "")

	# Generate the doxygen configuration file used by the
	# document generation. This step permits to use cmake variable
	# such as @ORIGINAL_PROJECT_NAME@ in the Doxyfile.in template
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_HOME_DIRECTORY}/Doxyfile)

	# The dox target will generate the doxygen documentation in ${DOXYGEN_BUILD_DIR}
	add_custom_target(doxygen COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_HOME_DIRECTORY}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
		COMMENT "Generating doxygen documentation in ${DOXYGEN_BUILD_DIR}")
endif()