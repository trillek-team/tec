find_package(PythonInterp)
find_program(MKDOCS_ABSPATH NAMES mkdocs)

include(GNUInstallDirs)

if (PYTHONINTERP_FOUND AND MKDOCS_ABSPATH)
	set(MKDOCS_BUILD_DIR "${CMAKE_HOME_DIRECTORY}/mkdocs" CACHE STRING "Build directory for mkdocs (absolute).")
	add_custom_target(mkdocs COMMAND ${MKDOCS_ABSPATH} build -c -s -v --site-dir ${MKDOCS_BUILD_DIR}
		WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY}
		COMMENT "Generating doxygen documentation in ${MKDOCS_BUILD_DIR} from ${CMAKE_HOME_DIRECTORY}")
endif()