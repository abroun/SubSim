#-------------------------------------------------------------------------------
INCLUDE( ${PROJECT_SOURCE_DIR}/cmake/utils.cmake )

#-------------------------------------------------------------------------------
# Include all the search paths for headers
INCLUDE_DIRECTORIES(
    .
    ${global_include_dirs} )

SET( srcFiles 
    MathUtils.cpp
    HighPrecisionTime.cpp 
    CommandLineParser.cpp
    Utils.cpp )

ADD_LIBRARY( common ${srcFiles} )

