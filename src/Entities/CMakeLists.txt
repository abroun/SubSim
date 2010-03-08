#-------------------------------------------------------------------------------
INCLUDE( ${PROJECT_SOURCE_DIR}/cmake/utils.cmake )

#-------------------------------------------------------------------------------
# Include all the search paths for headers
INCLUDE_DIRECTORIES(
    .
    ${global_include_dirs} )

SET( srcFiles 
    Entity.cpp
    Sub.cpp 
    CoordinateSystemAxes.cpp
    Buoy.cpp
    Gate.cpp 
    Pool.cpp
    FloorTarget.cpp
    XmlEntityParser.cpp )

ADD_LIBRARY( entities ${srcFiles} )

