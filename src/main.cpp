//------------------------------------------------------------------------------
// A simple simulator for the UWE group project
//------------------------------------------------------------------------------
#include <stdio.h>
#include <irrlicht/irrlicht.h>

#include "IrrMonitor.h"
#include "Common/MathUtils.h"
#include "Entities/Sub.h"
#include "Entities/CoordinateSystemAxes.h"
#include "Entities/Gate.h"
#include "Entities/Buoy.h"
#include "Entities/Pool.h"
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
    
    irr::gui::IGUIStaticText* pText = pGUIEnvironment->addStaticText( 
        L"Hello World!", irr::core::rect< irr::s32 >( 10, 10, 200, 40 ), true );
    
    // Create sub
    Sub sub;
    if ( !sub.Init( pSceneMgr ) )
    {
        fprintf( stderr, "Error: Unable to create sub\n" );
        return -1;
    }
    sub.SetYaw( MathUtils::DegToRad( 45.0f ) );
    sub.SetPosition( Vector( -4.0f, 0.0f, 0.0f ) );

    // Create axes to show coordinate system
    CoordinateSystemAxes axes;
    if ( !axes.Init( pSceneMgr ) )
    {
        fprintf( stderr, "Error: Unable to create coordinate axes\n" );
        return -1;
    }
    
    Gate gate;
    gate.Init( pSceneMgr );
    gate.SetPosition( Vector( 0.0f, 15.0f, 0.0f ) );
    
    Buoy buoy;
    buoy.Init( pSceneMgr );
    buoy.SetPosition( Vector( 0.0f, 5.0f, 0.0f ) );
    
    Pool pool;
    pool.Init( pSceneMgr );
    pool.SetPosition( Vector( 0.0f, -15.0f, 0.0f ) );
    
    // Create some fog to represent underwater visibility
    pVideoDriver->setFog( irr::video::SColor( 0,0,25,220 ), 
                          irr::video::EFT_FOG_EXP, 50, 3000, 0.005f, true, false );
    
    /*// Load pool
    irr::scene::IAnimatedMesh* pPoolMesh = pSceneMgr->getMesh( "media/export/pool.x" );
    //IrrMonitor<irr::scene::IAnimatedMesh> poolMeshMonitor( pPoolMesh );
    if ( NULL == pPoolMesh )
    {
        fprintf( stderr, "Error: Unable to load mesh\n" );
        return -1;
    }
    
    irr::scene::IAnimatedMeshSceneNode* pPoolNode = pSceneMgr->addAnimatedMeshSceneNode( pPoolMesh );
    if ( NULL != pPoolNode )
    {
        pPoolNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
    }*/
      
    // Setup the camera
    //pSceneMgr->addCameraSceneNode( 0, irr::core::vector3df( 0, 10, -10 ), irr::core::vector3df( 0, 0, 0 ) );
    irr::scene::ICameraSceneNode* pCamera = pSceneMgr->addCameraSceneNodeMaya( 0, -750.0f, 200.0f, 300.0f ); 
    pCamera->setTarget( irr::core::vector3df( 0, 0, 0 ) );
    
    S32 lastFPS = -1;
    while ( pIrrDevice->run() )
    {
        pVideoDriver->beginScene( true, true, irr::video::SColor( 255, 100, 101, 140 ) );
        
        pSceneMgr->drawAll();
        pGUIEnvironment->drawAll();
        
        pVideoDriver->endScene();
        
        // Update FPS counter
        S32 fps = pVideoDriver->getFPS();
        if ( lastFPS != fps )
        {
            irr::core::stringw str = L"Hello World - FPS: ";
            str += fps;
            
            pText->setText( str.c_str() );
            lastFPS = fps;
        }
    }
    
    // Clean up
    //sub.DeInit();
    //axes.DeInit();
    //pPoolMesh->drop();
    //pIrrDevice->drop();
    //pIrrDevice = NULL;

    return 0;
}
