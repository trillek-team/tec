CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT("TEC")
SET(CMAKE_CXX_STANDARD_REQUIRED 14)
SET(CMAKE_CXX_STANDARD 14)
# Set the directory of cmake modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules")

INCLUDE(ExternalProject)
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)
# Lib extension (used to link to ExternalProject
if(MSVC)
    set(LibSuffix ".lib")
else()
    set(LibSuffix ".a")
endif()

# Set the build type if it isn't already
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()

# Test not enabled by default
OPTION(BUILD_TESTS_TEC "Build TEC unit tests" FALSE)

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
  # This is needed for OpenAL as the libraries were built using an older MSVC.
  IF(${_MACHINE_ARCH_FLAG} MATCHES X86)
     SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
     SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
     SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
   ENDIF()
ENDIF ( MSVC )

# find all source files in the src directory
FILE(GLOB_RECURSE TEC_SRC "src/*.cpp")
LIST(REMOVE_ITEM TEC_SRC "${CMAKE_SOURCE_DIR}/src/main.cpp")

# find all the files in the includes directory
FILE(GLOB_RECURSE TEC_INCLUDES "include/*.h" "include/*.hpp")
# Filters for VStudio
SOURCE_GROUP("TEC" "src/.*[.]cpp")
SOURCE_GROUP("TEC" "include/.*[.]hpp")
SOURCE_GROUP("TEC\\Components" ".*/components/.*")
SOURCE_GROUP("TEC\\Physics" ".*/physics/.*")
SOURCE_GROUP("TEC\\Resources" ".*/resources/.*")
SOURCE_GROUP("TEC\\Graphics" ".*/graphics/.*")
SOURCE_GROUP("TEC\\GUI" ".*/gui/.*")
SOURCE_GROUP("TEC\\Controllers" ".*/controllers/.*")
SOURCE_GROUP("TEC\\Systems" ".*-system.*")

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# IMGUI setup
FILE(GLOB IMGUI_SRC
  "modules/imgui/imgui.cpp"
  "modules/imgui/imgui_draw.cpp"
  "modules/imgui/imgui_demo.cpp"
  )
FILE(GLOB IMGUI_INCLUDES "modules/imgui/*.h")
SOURCE_GROUP(ImGui FILES ${IMGUI_SRC} ${IMGUI_INCLUDES})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/modules/imgui/)


SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include/")

# SPDLOG setup
FILE(GLOB SPDLOG_INCLUDES
  "modules/spdlog/include/spdlog/*.h"
  "modules/spdlog/include/spdlog/details/*.h"
  "modules/spdlog/include/spdlog/sinks/*.h"
  )
LIST(REMOVE_ITEM SPDLOG_INCLUDES "modules/spdlog/include/spdlog/tweakme.h" )
SOURCE_GROUP(SPDLog FILES ${SPDLOG_INCLUDES})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/modules/spdlog/include/)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/include/")

# If we are on windows add in the local search directories as well.
IF (WIN32 AND NOT MINGW) # Windows
  SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/lib/include/)
  INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/lib/include/")
  LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/lib/")
  SET(CMAKE_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/lib/)
ENDIF (WIN32 AND NOT MINGW)

# This is used to flag if TEC should be built or if we are just doing a dependency build.
SET(BUILD_DEPS_FIRST false)

FIND_PACKAGE(OpenGL REQUIRED)

# GLFW3 check
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "modules/glfw3/include")
MESSAGE("GLFW3 will be built.")
# We are turning off all the extra glfw options before adding it to reduce compiling.
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
SET(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
ADD_SUBDIRECTORY("modules/glfw3")
INCLUDE_DIRECTORIES("modules/glfw3/include")
SET(GLFW_LIBRARIES glfw ${GLFW_LIBRARIES})

# PThread check
SET(CMAKE_THREAD_PREFER_PTHREAD TRUE)
FIND_PACKAGE( Threads REQUIRED)
IF(CMAKE_USE_PTHREADS_INIT)
  SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
ENDIF(CMAKE_USE_PTHREADS_INIT)

# GLM include
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "modules/glmi/glm/")
INCLUDE_DIRECTORIES("modules/glm")

# Protobuf setup
FILE(GLOB PB_FILES_SRC "proto/*.cc")
FILE(GLOB PB_FILES_INCLUDES "proto/*.h")
SET(BUILD_TESTING OFF CACHE BOOL "Build tests")
INCLUDE_DIRECTORIES("modules/protobuf/src")
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "modules/protobuf/src")
ADD_SUBDIRECTORY("modules/protobuf/cmake")

# Bullet
SET(BULLET_CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_DEBUG_POSTFIX=d
    -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON
    -DUSE_DOUBLE_PRECISION=ON
    -DBUILD_DEMOS=OFF
    -DBUILD_CPU_DEMOS=OFF
    -DBUILD_OPENGL3_DEMOS=OFF
    -DBUILD_BULLET2_DEMOS=OFF
    -DBUILD_BULLET3=OFF
    -DBUILD_EXTRAS=OFF
    -DBUILD_UNIT_TESTS=OFF
    -DBUILD_STATIC_LIBS=ON
)
ExternalProject_Add(
    Bullet
    DOWNLOAD_COMMAND ""
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/modules/bullet3/
    CMAKE_ARGS ${BULLET_CMAKE_ARGS}
    INSTALL_COMMAND ""
    LOG_CONFIGURE ON
    LOG_BUILD ON
    )
# Recover project paths for additional settings
ExternalProject_Get_Property(Bullet BINARY_DIR )

# Set properties as per FindModules
SET( BULLET_FOUND
    CACHE INTERNAL "" FORCE
)

SET( BULLET_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/modules/bullet3/src/"
    CACHE INTERNAL "" FORCE
)

SET( BULLET_LIBRARIES
    debug ${BINARY_DIR}/src/BulletSoftBody/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}BulletSoftBodyd${LibSuffix}
    optimized ${BINARY_DIR}/src/BulletSoftBody/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}BulletSoftBody${LibSuffix}
    debug ${BINARY_DIR}/src/BulletDynamics/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}BulletDynamicsd${LibSuffix}
    optimized ${BINARY_DIR}/src/BulletDynamics/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}BulletDynamics${LibSuffix}
    debug ${BINARY_DIR}/src/BulletCollision/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}BulletCollisiond${LibSuffix}
    optimized ${BINARY_DIR}/src/BulletCollision/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}BulletCollision${LibSuffix}
    debug ${BINARY_DIR}/src/LinearMath/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}LinearMathd${LibSuffix}
    optimized ${BINARY_DIR}/src/LinearMath/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}LinearMath${LibSuffix}
    CACHE INTERNAL "" FORCE
)

SET( BULLET_DEFINITIONS
    " -DBT_USE_DOUBLE_PRECISION "
    CACHE INTERNAL "" FORCE
)
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "modules/bullet3/src")
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/modules/bullet3/src/)

# OpenAL check
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "modules/openal-soft/include/AL")
FIND_PACKAGE(OpenAL)
IF (NOT ${OPENAL_FOUND})
  SET(BUILD_DEPS_FIRST true)
  MESSAGE("Bullet will be built.")
  SET(ALSOFT_UTILS OFF CACHE BOOL "Build and install utility programs")
  SET(ALSOFT_EXAMPLES OFF CACHE BOOL "Build and install example programs")
  SET(ALSOFT_REQUIRE_WINMM ON CACHE BOOL "Require Windows Multimedia backend")
  ADD_SUBDIRECTORY("modules/openal-soft")
  INCLUDE_DIRECTORIES("modules//openal-soft/include")
ELSE (NOT ${OPENAL_FOUND})
  MESSAGE("OpenAL already found skipping build.")
  INCLUDE_DIRECTORIES (${OPENAL_INCLUDE_DIR})
ENDIF (NOT ${OPENAL_FOUND})

# Add ASIO as it is header only
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "modules/asio/asio/include")
INCLUDE_DIRECTORIES("modules/asio/asio/include")

# Some definitions we need to use bullet, GLM, and ASIO.
ADD_DEFINITIONS( -DBT_USE_DOUBLE_PRECISION -DGLM_FORCE_PURE -DASIO_STANDALONE )

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
IF (NOT ${BUILD_DEPS_FIRST})
  MESSAGE("All dependencies found now building tec.")
  # Add the virtual computer
  SET(BUILD_STATIC_VCOMPUTER TRUE CACHE BOOL "Build Trillek VCOMPUTER library - static
       version")
  SET(BUILD_DYNAMIC_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER library - dynamic version")
  SET(BUILD_TOOLS_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER tools")
  SET(BUILD_TESTS_VCOMPUTER FALSE CACHE BOOL "Build Trillek VCOMPUTER tests")
  ADD_SUBDIRECTORY("modules/trillek-vcomputer")
  INCLUDE_DIRECTORIES("modules/trillek-vcomputer/include")

  SET(TEC_ALL_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENGL_LIBRARIES}
    ${X11_LIBRARIES}
    ${OSX_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${OPENAL_LIBRARY}
    libprotobuf
    VCOMPUTER_STATIC
  )

  ADD_LIBRARY(TEC_LIB STATIC
    ${TEC_SRC}
    ${TEC_INCLUDES}
    ${IMGUI_SRC}
    ${IMGUI_INCLUDES}
    ${SPDLOG_INCLUDES}
    ${PB_FILES_SRC}
    ${PB_FILES_INCLUDES}
  )
  ADD_DEPENDENCIES(TEC_LIB Bullet)

  ADD_EXECUTABLE("TEC"
    "src/main.cpp"
    "client/server_connection.hpp"
    "client/server_connection.cpp"
  )
  #target_include_directories(TEC PUBLIC "client")
  IF (WIN32)
    TARGET_COMPILE_DEFINITIONS( "TEC" PUBLIC _WIN32_WINNT=0x600 )
  ENDIF (WIN32)


  TARGET_LINK_LIBRARIES("TEC" TEC_LIB  ${TEC_ALL_LIBS})
  
  ADD_EXECUTABLE("Server"
    "server/main.cpp"
    "server/client_connection.hpp"
    "server/client_connection.cpp"
    "server/server.hpp"
    "server/server.cpp"
  )
  #target_include_directories("Server" PUBLIC "server")
  IF (WIN32)
    TARGET_COMPILE_DEFINITIONS( "Server" PUBLIC _WIN32_WINNT=0x600 )
  ENDIF (WIN32)

  TARGET_LINK_LIBRARIES("Server" TEC_LIB  ${TEC_ALL_LIBS})

  IF(BUILD_TESTS_TEC)
    ADD_SUBDIRECTORY("tests")
  ENDIF(BUILD_TESTS_TEC)

ENDIF (NOT ${BUILD_DEPS_FIRST})
