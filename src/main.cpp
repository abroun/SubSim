//------------------------------------------------------------------------------
// A simple simulator for the UWE group project
//------------------------------------------------------------------------------
#include <stdio.h>
#include <irrlicht/irrlicht.h>

#include "IrrMonitor.h"

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
    
    // Load mesh
    irr::scene::IAnimatedMesh* pMesh = pSceneMgr->getMesh( "media/export/sub.x" );
    if ( NULL == pMesh )
    {
        fprintf( stderr, "Error: Unable to load mesh\n" );
        return -1;
    }
    
    irr::scene::IAnimatedMeshSceneNode* pNode = pSceneMgr->addAnimatedMeshSceneNode( pMesh );
    IrrMonitor<irr::scene::IAnimatedMeshSceneNode> subNodeMonitor( pNode );
    if ( NULL != pNode )
    {
        pNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
    }
    
    // Create an arrow
    const irr::scene::IGeometryCreator* pCreator = pSceneMgr->getGeometryCreator();
    
    irr::scene::IMesh* pArrowMeshZ = pCreator->createArrowMesh( 
        4, 8, 1.0f, 0.6f, 0.05f, 0.1f, 
        irr::video::SColor( 255, 0, 0, 255 ), irr::video::SColor( 255, 0, 0, 255 ) ); 
    irr::scene::IMeshSceneNode* pArrowNodeZ = pSceneMgr->addMeshSceneNode( pArrowMeshZ );
    IrrMonitor<irr::scene::IMesh> arrowMeshZMonitor( pArrowMeshZ );    
    IrrMonitor<irr::scene::IMeshSceneNode> arrowNodeZMonitor( pArrowNodeZ );

    pArrowNodeZ->setPosition( irr::core::vector3df( 4.0f, 0.0f, 0.0f ) );
    pArrowNodeZ->setRotation( irr::core::vector3df( 90.0f, 0.0f, 0.0f ) );
    pArrowNodeZ->setMaterialFlag( irr::video::EMF_LIGHTING, false );
    
    irr::scene::IMesh* pArrowMeshY = pCreator->createArrowMesh( 
        4, 8, 1.0f, 0.6f, 0.05f, 0.1f, 
        irr::video::SColor( 255, 0, 255, 0 ), irr::video::SColor( 255, 0, 255, 0 ) ); 
    irr::scene::IMeshSceneNode* pArrowNodeY = pSceneMgr->addMeshSceneNode( pArrowMeshY );
    IrrMonitor<irr::scene::IMesh> arrowMeshYMonitor( pArrowMeshY );    
    IrrMonitor<irr::scene::IMeshSceneNode> arrowNodeYMonitor( pArrowNodeY );

    pArrowNodeY->setPosition( irr::core::vector3df( 4.0f, 0.0f, 0.0f ) );
    pArrowNodeY->setRotation( irr::core::vector3df( 0.0f, 0.0f, 0.0f ) );
    pArrowNodeY->setMaterialFlag( irr::video::EMF_LIGHTING, false );
    
    irr::scene::IMesh* pArrowMeshX = pCreator->createArrowMesh( 
        4, 8, 1.0f, 0.6f, 0.05f, 0.1f, 
        irr::video::SColor( 255, 255, 0, 0 ), irr::video::SColor( 255, 255, 0, 0 ) ); 
    irr::scene::IMeshSceneNode* pArrowNodeX = pSceneMgr->addMeshSceneNode( pArrowMeshX );
    IrrMonitor<irr::scene::IMesh> arrowMeshXMonitor( pArrowMeshX );    
    IrrMonitor<irr::scene::IMeshSceneNode> arrowNodeXMonitor( pArrowNodeX );

    
    pArrowNodeX->setPosition( irr::core::vector3df( 4.0f, 0.0f, 0.0f ) );
    pArrowNodeX->setRotation( irr::core::vector3df( 0.0f, 0.0f, -90.0f ) );
    pArrowNodeX->setMaterialFlag( irr::video::EMF_LIGHTING, false );
    
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
