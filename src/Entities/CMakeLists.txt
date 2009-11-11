#-------------------------------------------------------------------------------
INCLUDE( ${PROJECT_SOURCE_DIR}/cmake/utils.cmake )

#-------------------------------------------------------------------------------
# Include all the search paths for headers
INCLUDE_DIRECTORIES(
    .
    ${global_include_dirs} )

SET( entitySrcFiles 
    Sub.cpp 
    CoordinateSystemAxes.cpp )

ADD_LIBRARY( entities ${entitySrcFiles} )

