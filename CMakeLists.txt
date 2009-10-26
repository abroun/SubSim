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
SET( IRRLICHT_VERSION 1.6 CACHE INTERNAL "Irrlicht version requirement" FORCE )
SET( IRRLICHT_LIBRARY_PATH "/usr/local/lib" CACHE INTERNAL "Irrlicht library path" )

IF ( PKG_CONFIG_FOUND )

    pkg_check_modules( IRRLICHT Irrlicht>=${IRRLICHT_VERSION} )
    IF ( NOT IRRLICHT_FOUND )
        MESSAGE (SEND_ERROR "\nError: Irrlicht version >=${IRRLICHT_VERSION} and development files not found.")
    ELSE ( NOT IRRLICHT_FOUND )

        SET( IRRLICHT_LIBRARY_PATH ${IRRLICHT_LIBRARY_DIRS} CACHE INTERNAL "Irrlicht library path" )
        APPEND_LIBRARY_VARS_TO_CACHED_LISTS( ${IRRLICHT_INCLUDE_DIRS}
                                ${IRRLICHT_LIBRARY_DIRS}
                                ${IRRLICHT_LINK_LIBS}
                                ${IRRLICHT_LIBRARIES}
                                ${IRRLICHT_LDFLAGS} )
    ENDIF ( NOT IRRLICHT_FOUND )

ELSE ( PKG_CONFIG_FOUND )

  SET( BUILD_SUBSIM OFF CACHE INTERNAL "Build SubSim" FORCE )
  MESSAGE( FATAL_ERROR "\nError: pkg-config not found" )

ENDIF ( PKG_CONFIG_FOUND )

#-------------------------------------------------------------------------------
# Include the source files

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR} 
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/include )

ADD_SUBDIRECTORY( src )

# Copy data into the build directory
RECURSIVELY_COPY_DIRECTORY( ${PROJECT_SOURCE_DIR}/media/export ${PROJECT_BINARY_DIR}/media/export )
  
MESSAGE( STATUS "Build Type: ${CMAKE_BUILD_TYPE}" )
