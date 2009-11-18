#-------------------------------------------------------------------------------
INCLUDE( ${PROJECT_SOURCE_DIR}/cmake/utils.cmake )

#-------------------------------------------------------------------------------
# Include all the search paths for headers
INCLUDE_DIRECTORIES(
    .
    ${global_include_dirs} )

SET( srcFiles 
    SubSimDriver.cpp
    SubSimInterface.cpp
    SimulationInterface.cpp )

LINK_DIRECTORIES( ${global_link_dirs} )
ADD_LIBRARY( subsimplugin SHARED ${srcFiles} )

TARGET_LINK_LIBRARIES( subsimplugin ${global_link_libs} )

