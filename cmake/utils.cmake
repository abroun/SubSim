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


#-------------------------------------------------------------------------------
