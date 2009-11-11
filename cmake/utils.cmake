#-------------------------------------------------------------------------------
# Utility routines
# Some taken from Gazebo CMake files
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# APPEND_TO_CACHED_LIST (_list _cacheDesc [items...]
# Appends items to a cached list.
MACRO( APPEND_TO_CACHED_LIST _list _cacheDesc )
  SET (tempList ${${_list}})
  FOREACH (newItem ${ARGN})
    LIST (APPEND tempList ${newItem})
  ENDFOREACH (newItem ${newItem})
  SET (${_list} ${tempList} CACHE INTERNAL ${_cacheDesc} FORCE)
ENDMACRO(APPEND_TO_CACHED_LIST)

#-------------------------------------------------------------------------------
# Initialise the lists
MACRO( INIT_LISTS )
    SET( global_sources_desc "List of sources" 
         CACHE INTERNAL "Sources list description" FORCE )
    SET( global_headers_desc "List of headers" 
         CACHE INTERNAL "Headers list description" FORCE )
    SET( global_include_dirs_desc "List of include directories" 
         CACHE INTERNAL "Include dirs description" )
    SET( global_link_dirs_desc "List of link directories" 
         CACHE INTERNAL "Link dirs description" FORCE )
    SET( global_link_libs_desc "List of link libraries" 
         CACHE INTERNAL "Link libs description" FORCE )
    SET( global_cflags_desc "Compiler flags" 
         CACHE INTERNAL "Compiler flags description" FORCE )
ENDMACRO( INIT_LISTS )

#-------------------------------------------------------------------------------
MACRO( RESET_LISTS )
    SET( global_sources "" CACHE INTERNAL 
        ${global_sources_desc} FORCE )
    SET( global_headers "" CACHE INTERNAL 
        ${global_sources_desc} FORCE )
    SET( global_include_dirs "" CACHE INTERNAL 
        ${global_include_dirs_desc} FORCE )
    SET( global_link_dirs "" CACHE INTERNAL 
        ${global_link_dirs_desc} FORCE )
    SET( global_link_libs "" CACHE INTERNAL 
        ${global_link_libs_desc} FORCE )
    SET( global_cflags "" CACHE INTERNAL 
        ${global_cflags_desc} FORCE )
ENDMACRO( RESET_LISTS )

#-------------------------------------------------------------------------------
MACRO( APPEND_LIBRARY_VARS_TO_CACHED_LISTS _include_dirs _library_dirs _link_libs _libraries _ld_flags )
    APPEND_TO_CACHED_LIST( global_include_dirs 
                           ${global_include_dirs_desc} 
                           ${_include_dirs} )
    APPEND_TO_CACHED_LIST( global_link_dirs 
                           ${global_link_dirs_desc} 
                           ${_library_dirs} )
    APPEND_TO_CACHED_LIST( global_link_libs 
                           ${global_link_libs_desc} 
                           ${_link_libs} )
    APPEND_TO_CACHED_LIST( global_link_libs 
                           ${global_link_libs_desc} 
                           ${_libraries} )
    APPEND_TO_CACHED_LIST( global_link_libs 
                           ${global_link_libs_desc} 
                           ${_ld_flags} )
ENDMACRO( APPEND_LIBRARY_VARS_TO_CACHED_LISTS )

#-------------------------------------------------------------------------------
# Macro to turn a list into a string (why doesn't CMake have this built-in?)
MACRO( LIST_TO_STRING _string _list )
    SET( ${_string} )
    FOREACH( _item ${_list} )
      SET( ${_string} "${${_string}} ${_item}" )
    ENDFOREACH( _item )
    #STRING(STRIP ${${_string}} ${_string})
ENDMACRO( LIST_TO_STRING )

#-------------------------------------------------------------------------------
# Configure time copying of directories. Use with care as it's pretty 
# inefficient
MACRO( RECURSIVELY_COPY_DIRECTORY _srcDir _destDir)
    MESSAGE( STATUS "Copying directory ${_srcDir}" )
    FILE( MAKE_DIRECTORY ${_destDir} )

    # Get a list of all files below the source directory (this seems to be the only way
    # to get a list of subdirectories)
    FILE( GLOB_RECURSE subFileList ${_srcDir}/* )

    FOREACH ( subFile ${subFileList} )

        # Get the length of the source dir, have to refresh it each time as it 
        # may be changed by other calls to RECURSIVELY_COPY_DIRECTORY
        STRING( LENGTH ${_srcDir} srcDirLength )
        MATH( EXPR srcDirLength "${srcDirLength} + 1" ) # Skip trailing slash - may fail if it's not there

        # Get the length of the string without the source directory  
        STRING( LENGTH ${subFile} subFileLength )
        MATH( EXPR subStringLength "${subFileLength} - ${srcDirLength}" )
        
        # Remove the source directory from the file name
        STRING( SUBSTRING ${subFile} ${srcDirLength} ${subStringLength} subString )

        STRING( REGEX MATCH "^[^/]*" subDirString ${subString} )
        STRING( LENGTH ${subDirString} subDirStringLength )

        IF ( ${subDirStringLength} LESS ${subStringLength} )
            
            #MESSAGE( STATUS "Copying ${_srcDir}/${subDirString}" )
            RECURSIVELY_COPY_DIRECTORY( ${_srcDir}/${subDirString} ${_destDir}/${subDirString} )

        ELSE ( ${subDirStringLength} LESS ${subStringLength} )

            #MESSAGE( STATUS "Copying ${_srcDir}/${subString}" )
            EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy ${_srcDir}/${subString} ${_destDir}/${subString} )

        ENDIF ( ${subDirStringLength} LESS ${subStringLength} )

    ENDFOREACH ( subFile )

ENDMACRO( RECURSIVELY_COPY_DIRECTORY )

#-------------------------------------------------------------------------------
MACRO( ADD_CXXTEST _name )
  IF ( PYTHONINTERP_FOUND )
    #file( MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/unitTests )
    ADD_CUSTOM_COMMAND(
      OUTPUT ${PROJECT_BINARY_DIR}/${_name}.cpp
      COMMAND
        ${PYTHON_EXECUTABLE} ${CXXTESTGEN}
        --runner=ErrorPrinter
        -o ${PROJECT_BINARY_DIR}/${_name}.cpp ${ARGN}
      DEPENDS ${ARGN}
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
  ENDIF ( PYTHONINTERP_FOUND )

  

# This 'cmake_policy' line stops a warning generated by 'TARGET_LINK_LIBRARIES'
# below. Comment it out to see the warning
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
ADD_EXECUTABLE(${_name} ${PROJECT_BINARY_DIR}/${_name}.cpp ${ARGN})
  TARGET_LINK_LIBRARIES( ${_name} ${global_link_libs} entities mathUtils )

  ADD_TEST(${_name} ${_name})
ENDMACRO ( ADD_CXXTEST )



#-------------------------------------------------------------------------------
