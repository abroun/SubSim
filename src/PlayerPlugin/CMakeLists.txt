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
    SimulationInterface.cpp
    Position3DInterface.cpp
    CameraInterface.cpp
    CompassInterface.cpp
    DepthSensorInterface.cpp )

LINK_DIRECTORIES( ${global_link_dirs} )
ADD_LIBRARY( subsimplugin SHARED ${srcFiles} )
SET_TARGET_PROPERTIES( subsimplugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )
# delete the "lib" prefix from the plugin
SET_TARGET_PROPERTIES( subsimplugin PROPERTIES PREFIX "" )

TARGET_LINK_LIBRARIES( subsimplugin
    simulator 
    entities 
    common 
    ${global_link_libs} 
    rt 
    BulletDynamics
    BulletCollision
    LinearMath    # LinearMath is also from Bullet
    xerces-c-3.1
    Irrlicht )

INSTALL( TARGETS subsimplugin
        LIBRARY DESTINATION lib )
