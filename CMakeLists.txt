CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT("TEC")

# Set the directory of cmake modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules")

# Put the executable in the bin folder
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
IF ( MSVC )
	# If using MSVC you must change the 'Debugging->Working Directory' property in
	# the project properties window to "$(SolutionDir)..\" for assets to load correctly.
	SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )
	SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib )
	# Second, for multi-config builds (e.g. msvc)
	foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
		STRING( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
		SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib )
		SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib )
	endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
ENDIF ( MSVC )

# find all source files in the src directory
FILE(GLOB_RECURSE TEC_SRC "src/*.cpp")

# find all the files in the includes directory
FILE(GLOB_RECURSE TEC_INCLUDES "include/*.h" "include/*.hpp")

#Check if we can build with the current tool chain
INCLUDE(SetCompilerFlags)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include/")

# If we are on windows add in the local search directories as well.
IF (WIN32 AND NOT MINGW) # Windows
	SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/lib/include/)
	INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/lib/include/")
	LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/lib/")
	SET(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/)
ENDIF (WIN32 AND NOT MINGW)

FIND_PACKAGE(OpenGL REQUIRED)

ADD_SUBDIRECTORY("modules/glfw3")
INCLUDE_DIRECTORIES("modules/glfw3/include")
ADD_SUBDIRECTORY("modules/glm")
INCLUDE_DIRECTORIES("modules/glm")
ADD_SUBDIRECTORY("modules/bullet3")
INCLUDE_DIRECTORIES("modules/bullet3/src")
FIND_PACKAGE(Bullet)

# Give these some dummy values and if the platform is LINUX or OSX they will be set accordingly.
SET(X11_LIBRARIES "")
SET(OSX_LIBRARIES "")

IF (NOT APPLE) # X11 and GLEW are not needed on OSX.
	FIND_PACKAGE(X11)
	FIND_PACKAGE(GLEW REQUIRED) # We find GLEW here as OSX doesn't need it.
ENDIF (NOT APPLE)

IF (APPLE) # Mac OSX
	SET(GLEW_LIBRARY "") # Set a dummy value for GLEW.

	SET(CMAKE_XCODE_ATTRIBUTE_SUPPORTED_PLATFORMS macosx)

	# Need the 10.7 SDK or later.
	EXECUTE_PROCESS(COMMAND xcodebuild -sdk macosx -version SDKVersion OUTPUT_VARIABLE OSX_SDK_VERSION)
	IF (NOT (OSX_SDK_VERSION VERSION_GREATER 10.7 OR OSX_SDK_VERSION VERSION_EQUAL 10.7))
		MESSAGE(FATAL_ERROR "The installed version of Xcode does not support the 10.7 SDK or later.  Please upgrade Xcode and try again.")
	ENDIF (NOT (OSX_SDK_VERSION VERSION_GREATER 10.7 OR OSX_SDK_VERSION VERSION_EQUAL 10.7))

	# Configure the project to use the correct SDK.
	IF (XCODE_VERSION)
		SET(CMAKE_OSX_SYSROOT macosx)
	ELSE (XCODE_VERSION)
		# Non-Xcode generators need the full path.
		EXECUTE_PROCESS(COMMAND xcodebuild -sdk macosx -version Path | head -n 1 OUTPUT_VARIABLE CMAKE_OSX_SYSROOT)
		STRING(REGEX REPLACE "(\r?\n)+$" "" CMAKE_OSX_SYSROOT "${CMAKE_OSX_SYSROOT}")
	ENDIF (XCODE_VERSION)

	# Can deploy back to 10.7, the first OS X to support the GL Core.
	SET(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)

	# Need Core Foundation and libobjc.
	SET(OSX_LIBRARIES "-framework CoreFoundation /usr/lib/libobjc.dylib")
ENDIF (APPLE)

# define all required external libraries
SET(TEC_ALL_LIBS
	${OPENGL_LIBRARIES}
	${X11_LIBRARIES}
	${OSX_LIBRARIES}
	${GLEW_LIBRARIES}
	glfw ${GLFW_LIBRARIES}
	${BULLET_DYNAMICS_LIBRARY}
	${BULLET_COLLISION_LIBRARY}
	${BULLET_MATH_LIBRARY}
)

ADD_EXECUTABLE("TEC"
	${TEC_SRC}
	${TEC_INCLUDES}
)

TARGET_LINK_LIBRARIES("TEC" ${TEC_ALL_LIBS})
