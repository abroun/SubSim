//------------------------------------------------------------------------------
// A simple simulator for the UWE group project
//------------------------------------------------------------------------------
#include <stdio.h>
#include <irrlicht/irrlicht.h>

#include "IrrMonitor.h"
#include "Sub.h"
#include "CoordinateSystemAxes.h"
#include "Vector.h"

//------------------------------------------------------------------------------
int main()
{   
    irr::IrrlichtDevice* pIrrDevice = irr::createDevice( 
        irr::video::EDT_OPENGL, 
        irr::core::dimension2d<irr::u32>( 640, 480 ), 
        16, false, false, false, 0 );
    IrrMonitor<irr::IrrlichtDevice> irrDeviceMonitor( pIrrDevice );

    if ( NULL == pIrrDevice )
    {
        fprintf( stderr, "Error: Unable to create Irrlicht Device\n" );
        return -1;
    }

    pIrrDevice->setWindowCaption( L"SubSim" );
    
    irr::video::IVideoDriver* pVideoDriver = pIrrDevice->getVideoDriver();
    irr::scene::ISceneManager* pSceneMgr = pIrrDevice->getSceneManager();
    irr::gui::IGUIEnvironment* pGUIEnvironment = pIrrDevice->getGUIEnvironment();
    
    pGUIEnvironment->addStaticText( L"Hello World!", 
                                    irr::core::rect< irr::s32 >( 10, 10, 200, 40 ), true );
    
    // Create sub
    Sub sub;
    if ( !sub.Init( pSceneMgr ) )
    {
        fprintf( stderr, "Error: Unable to create sub\n" );
        return -1;
    }

    // Create axes to show coordinate system
    CoordinateSystemAxes axes;
    if ( !axes.Init( pSceneMgr ) )
    {
        fprintf( stderr, "Error: Unable to create coordinate axes\n" );
        return -1;
    }
    
    // Load pool
    irr::scene::IAnimatedMesh* pPoolMesh = pSceneMgr->getMesh( "media/export/pool.x" );
    if ( NULL == pPoolMesh )
    {
        fprintf( stderr, "Error: Unable to load mesh\n" );
        return -1;
    }
    
    irr::scene::IAnimatedMeshSceneNode* pPoolNode = pSceneMgr->addAnimatedMeshSceneNode( pPoolMesh );
    IrrMonitor<irr::scene::IAnimatedMeshSceneNode> poolNodeMonitor( pPoolNode );
    if ( NULL != pPoolNode )
    {
        pPoolNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
    }
      
    // Setup the camera
    pSceneMgr->addCameraSceneNode( 0, irr::core::vector3df( 0, 10, -10 ), irr::core::vector3df( 0, 0, 0 ) );
                                    
    while ( pIrrDevice->run() )
    {
        pVideoDriver->beginScene( true, true, irr::video::SColor( 255, 100, 101, 140 ) );
        
        pSceneMgr->drawAll();
        pGUIEnvironment->drawAll();
        
        pVideoDriver->endScene();
    }
    
    // Clean up
    pIrrDevice->drop();
    pIrrDevice = NULL;

    return 0;
}
