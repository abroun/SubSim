#-------------------------------------------------------------------------------
INCLUDE( ${PROJECT_SOURCE_DIR}/cmake/utils.cmake )

#-------------------------------------------------------------------------------
# Include all the search paths for headers
INCLUDE_DIRECTORIES(
    .
    ${global_include_dirs} )

LINK_DIRECTORIES(  
    ${global_link_dirs} )

SET( srcFiles 
    main.cpp 
    Sub.cpp 
    CoordinateSystemAxes.cpp )

ADD_EXECUTABLE( subsim ${srcFiles} )
SET_TARGET_PROPERTIES( subsim PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )

LIST_TO_STRING( SUBSIM_CFLAGS "${global_cflags}" )

SET_SOURCE_FILES_PROPERTIES( ${global_sources} 
    PROPERTIES COMPILE_FLAGS "${OGRETEST_CFLAGS} ${CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE}}" )

IF ( CMAKE_LINK_FLAGS_${CMAKE_BUILD_TYPE} )
  SET_SOURCE_FILES_PROPERTIES(${global_sources} PROPERTIES LINK_FLAGS 
    ${CMAKE_LINK_FLAGS_${CMAKE_BUILD_TYPE}})
ENDIF ( CMAKE_LINK_FLAGS_${CMAKE_BUILD_TYPE} )

TARGET_LINK_LIBRARIES( subsim ${global_link_libs} )
