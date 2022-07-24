find_program(DOXYBOOK_ABSPATH NAMES doxybook2)

if (DOXYBOOK_ABSPATH)
	set(DOXYBOOK_BUILD_DIR "${CMAKE_HOME_DIRECTORY}/docs/engine" CACHE STRING "Build directory for doxybook (absolute).")
	add_custom_target(doxybook COMMAND ${DOXYBOOK_ABSPATH}
		--input ${DOXYGEN_BUILD_DIR}/xml
		--output ${DOXYBOOK_BUILD_DIR}
		--config ${CMAKE_HOME_DIRECTORY}/.doxybook/config.json
		WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY} 
		COMMENT "Using doxybook to convert doxygen to markdown in ${DOXYBOOK_BUILD_DIR} from ${DOXYGEN_BUILD_DIR}")
endif()