#-------------------------------------------------------------------------------
# SubSim CMake file
#-------------------------------------------------------------------------------
CMAKE_MINIMUM_REQUIRED( VERSION 2.4.6 FATAL_ERROR )
PROJECT( SubSim )

INCLUDE( FindPkgConfig )
INCLUDE( ${PROJECT_SOURCE_DIR}/cmake/utils.cmake )

INIT_LISTS()
RESET_LISTS()

#-------------------------------------------------------------------------------
# Build type cflags
SET(CMAKE_C_FLAGS_RELEASE " -O3 -DNDEBUG -Wall " CACHE INTERNAL "C Flags for release" FORCE )
SET(CMAKE_C_FLAGS_DEBUG " -ggdb " CACHE INTERNAL "C Flags for debug" FORCE )

#-------------------------------------------------------------------------------
# Build type link flags
SET( CMAKE_LINK_FLAGS_RELEASE "" CACHE INTERNAL "Link flags for release" FORCE )
SET( CMAKE_LINK_FLAGS_DEBUG "" CACHE INTERNAL "Link flags for debug" FORCE )

#-------------------------------------------------------------------------------
# Set the default build type
IF( NOT CMAKE_BUILD_TYPE )
  SET ( CMAKE_BUILD_TYPE "release" CACHE STRING 
    "Choose the type of build, options are: debug release" FORCE )
ENDIF( NOT CMAKE_BUILD_TYPE )
STRING( TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE )

#-------------------------------------------------------------------------------
# Look for libraries we depend on
SET( PLAYER_VERSION 3.0.0 CACHE INTERNAL "Player version requirement" FORCE )

IF ( PKG_CONFIG_FOUND )

    pkg_check_modules( PLAYER playercore>=${PLAYER_VERSION} )
    IF ( NOT PLAYER_FOUND )
        MESSAGE (SEND_ERROR "\nError: Player version >=${PLAYER_VERSION} and development files not found.")
    ELSE ( NOT PLAYER_FOUND )

        SET( PLAYER_LIBRARY_PATH ${PLAYER_LIBRARY_DIRS} CACHE INTERNAL "Player library path" )
        APPEND_LIBRARY_VARS_TO_CACHED_LISTS( ${PLAYER_INCLUDE_DIRS}
                                ${PLAYER_LIBRARY_DIRS}
                                ${PLAYER_LINK_LIBS}
                                ${PLAYER_LIBRARIES}
                                ${PLAYER_LDFLAGS} )
    ENDIF ( NOT PLAYER_FOUND )

ELSE ( PKG_CONFIG_FOUND )

  SET( BUILD_SUBSIM OFF CACHE INTERNAL "Build SubSim" FORCE )
  MESSAGE( FATAL_ERROR "\nError: pkg-config not found" )

ENDIF ( PKG_CONFIG_FOUND )

#-------------------------------------------------------------------------------
# Add in CxxTest unit tests
ENABLE_TESTING()

# Use Python interpreter
FIND_PACKAGE(PythonInterp)

SET(CXXTESTGEN /home/abroun/dev/extra/cxxtest/cxxtestgen.py)

ADD_CXXTEST( unitTests 
            ${PROJECT_SOURCE_DIR}/unitTests/VectorTests.h
            ${PROJECT_SOURCE_DIR}/unitTests/MathUtilsTests.h 
            ${PROJECT_SOURCE_DIR}/unitTests/CommandLineParserTests.h )

#-------------------------------------------------------------------------------
# Include the source files

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR} 
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/3rdParty/cxxtest
    ${PROJECT_SOURCE_DIR}/../../install/include
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    /usr/include/irrlicht
    ${global_include_dirs} )

ADD_SUBDIRECTORY( src )

# Copy data into the build directory
RECURSIVELY_COPY_DIRECTORY( ${PROJECT_SOURCE_DIR}/media/export ${PROJECT_BINARY_DIR}/media/export )
  
MESSAGE( STATUS "Build Type: ${CMAKE_BUILD_TYPE}" )
