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

# GLFW3 check
FIND_PACKAGE(GLFW3)
IF (NOT ${GLFW_FOUND})
	MESSAGE("GLFW3 will be built.")
	# We are turning off all the extra bullet options before adding it to reduce compiling.
	SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
	SET(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
	SET(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
	SET(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
	ADD_SUBDIRECTORY("modules/glfw3")
	INCLUDE_DIRECTORIES("modules/glfw3/include")
ELSE (NOT ${GLFW_FOUND})
	MESSAGE("GLFW3 already found skipping build.")
	INCLUDE_DIRECTORIES (${GLFW_INCLUDE_DIR})
ENDIF (NOT ${GLFW_FOUND})

# GLM include
INCLUDE_DIRECTORIES("modules/glm")

# Bullet check
FIND_PACKAGE(Bullet)
IF (NOT ${BULLET_FOUND})
	MESSAGE("Bullet will be built.")
	# We are turning off all the extra bullet options before adding it to reduce compiling.
	SET(USE_DOUBLE_PRECISION ON CACHE BOOL "Use double precision")
	SET(USE_GLUT OFF CACHE BOOL "Use Glut")
	SET(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "Use Graphical Benchmark")
	SET(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "Set when you want to build the OpenGL3+ demos")
	SET(BUILD_BULLET2_DEMOS OFF CACHE BOOL "Set when you want to build the Bullet 2 demos")
	SET(BUILD_EXTRAS OFF CACHE BOOL "Set when you want to build the extras")
	SET(BUILD_UNIT_TESTS OFF CACHE BOOL "Build Unit Tests")
	SET(BUILD_CPU_DEMOS OFF CACHE BOOL "Build original Bullet CPU examples" )
	SET(BUILD_BULLET3 OFF CACHE BOOL "Set when you want to build Bullet 3")
	ADD_SUBDIRECTORY("modules/bullet3")
	INCLUDE_DIRECTORIES("modules/bullet3/src")
	SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "modules/bullet3/src")
ELSE (NOT ${BULLET_FOUND})
	MESSAGE("Bullet already found skipping build.")
	INCLUDE_DIRECTORIES (${BULLET_INCLUDE_DIRS})
ENDIF (NOT ${BULLET_FOUND})

# Some definitions we need to use bullet and GLM.
ADD_DEFINITIONS( -DBT_USE_DOUBLE_PRECISION -DGLM_FORCE_PURE)

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
# the use of raw names allows MSVC to pick-up debug version automatically
SET(TEC_ALL_LIBS
	${OPENGL_LIBRARIES}
	${X11_LIBRARIES}
	${OSX_LIBRARIES}
	${GLEW_LIBRARIES}
	${GLFW_LIBRARIES}
	${BULLET_LIBRARIES}
)

ADD_EXECUTABLE("TEC"
	${TEC_SRC}
	${TEC_INCLUDES}
)

TARGET_LINK_LIBRARIES("TEC" ${TEC_ALL_LIBS})
