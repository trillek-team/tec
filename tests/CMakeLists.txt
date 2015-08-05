CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
# Unit Tests
# idea take from : http://mifrosu.blogspot.com.es/2013/02/cmake-and-google-test-framework.html
# and from http://stackoverflow.com/questions/9689183/cmake-googletest

INCLUDE(ExternalProject)
# Set default ExternalProject root directory
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

FIND_PACKAGE(Threads REQUIRED)

# Set the build type if it isn't already
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()
# Add gtest
ExternalProject_Add(
    googletest
# SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
# SVN_REVISION -r 660
    URL http://googletest.googlecode.com/files/gtest-1.7.0.zip
    TIMEOUT 30
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
        -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
        #-Dgtest_force_shared_crt=ON
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    )
# Specify include dir
ExternalProject_Get_Property(googletest source_dir)


FILE(GLOB UNIT_TEST_FILES_SRC
  "*_test.cpp"
  )

INCLUDE_DIRECTORIES( UNIT_TEST
#    "${CMAKE_SOURCE_DIR}/src/"
#    "${CMAKE_SOURCE_DIR}/include/"
    "${CMAKE_SOURCE_DIR}/tests/"
    ${source_dir}/include
    )

ADD_EXECUTABLE( UNIT_TEST
    ${UNIT_TEST_FILES_SRC}
#    ${TEC_SRC}
    ${TEC_INCLUDES}
#    ${IMGUI_SRC}
#    ${IMGUI_INCLUDES}
#    ${PB_FILES_SRC}
#    ${PB_FILES_INCLUDES}
    )

ADD_DEPENDENCIES(UNIT_TEST googletest)

SET_PROPERTY(TARGET UNIT_TEST PROPERTY CXX_STANDARD 11)

ExternalProject_Get_Property(googletest binary_dir)
if(MSVC)
  set(GSuffix ".lib")
else()
  set(GSuffix ".a")
endif()

TARGET_LINK_LIBRARIES( UNIT_TEST
    debug ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${GSuffix}
    optimized ${binary_dir}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${GSuffix}
    debug ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${GSuffix}
    optimized ${binary_dir}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${GSuffix}
    TEC_LIB
    ${TEC_ALL_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    )
