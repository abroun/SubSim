//------------------------------------------------------------------------------
// File: Sub.cpp
// Desc: The submarine entity
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "Sub.h"
#include "Common/MathUtils.h"

//------------------------------------------------------------------------------
Sub::Sub()
    : mbInitialised( false ),
    mpConeMesh( NULL ),
    mpBodyMesh( NULL ),
    mpConeMeshNode( NULL ),
    mpBodyMeshNode( NULL )
{
}

//------------------------------------------------------------------------------
Sub::~Sub()
{
    DeInit();
}

//------------------------------------------------------------------------------
bool Sub::Init( irr::scene::ISceneManager* pSceneManager )
{
    if ( !mbInitialised )
    {
        if ( !Entity::Init( pSceneManager ) )
        {
            DeInit();
            return false;
        }
        
        // Create a cone and cylinder for the sub
        const irr::scene::IGeometryCreator* pCreator = pSceneManager->getGeometryCreator();

        mpConeMesh = pCreator->createConeMesh( 2, 6, 16, irr::video::SColor( 255, 255, 255, 0 ) );
        if ( NULL == mpConeMesh )
        {
            fprintf( stderr, "Error: Unable to create cone mesh" );
            DeInit();
            return false;
        }
        
        mpBodyMesh = pCreator->createCylinderMesh( 2, 3, 16, irr::video::SColor( 255, 255, 192, 0 ) );
        if ( NULL == mpBodyMesh )
        {
            fprintf( stderr, "Error: Unable to create body mesh" );
            DeInit();
            return false;
        }
        
        mpConeMeshNode = pSceneManager->addMeshSceneNode( mpConeMesh );
        if ( NULL == mpConeMeshNode )
        {
            fprintf( stderr, "Error: Unable to create cone mesh node" );
            DeInit();
            return false;
        }
        mpConeMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpConeMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );

        
        mpBodyMeshNode = pSceneManager->addMeshSceneNode( mpBodyMesh );
        if ( NULL == mpBodyMeshNode )
        {
            fprintf( stderr, "Error: Unable to create body mesh node" );
            DeInit();
            return false;
        }
        mpBodyMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpBodyMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        
        // Correctly position the sub model
        mpConeMeshNode->setRotation( irr::core::vector3df( 90.0f, 0.0f, 0.0f ) );
        mpConeMeshNode->setPosition( irr::core::vector3df( 0.0f, 0.0f, 3.0f ) );
        mpBodyMeshNode->setRotation( irr::core::vector3df( 90.0f, 0.0f, 0.0f ) );
        
        // Put the nodes under the control of SubSim
        AddChildNode( mpConeMeshNode );
        AddChildNode( mpBodyMeshNode );
        
        mForwardSpeed = 0.0f;
        mYawSpeed = 0.0f;

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void Sub::DeInit()
{
    RemoveAllChildNodes();
    
    if ( NULL != mpConeMeshNode )
    {
        mpConeMeshNode = NULL;
    }

    if ( NULL != mpConeMesh )
    {
        mpConeMesh->drop();
        mpConeMesh = NULL;
    }
    
    if ( NULL != mpBodyMeshNode )
    {
        mpBodyMeshNode = NULL;
    }

    if ( NULL != mpBodyMesh )
    {
        mpBodyMesh->drop();
        mpBodyMesh = NULL;
    }
    
    Entity::DeInit();

    mbInitialised = false;
}

//------------------------------------------------------------------------------
void Sub::Update( S32 timeStepMS )
{
    // For now we assume that the motion is linear and that forward motion
    // happens before changes in heading
    
    // Update the position of the sub
    F32 oldYaw = GetYaw();
    Vector heading( -(F32)sin( oldYaw ), (F32)cos( oldYaw ), 0.0f );
    Vector newPos = GetPosition() + (heading * mForwardSpeed * (F32)timeStepMS) / 1000.0f;
    
    F32 newYaw = oldYaw + (mYawSpeed * (F32)timeStepMS) / 1000.0f;
    
    SetPosition( newPos );
    SetYaw( newYaw );
}

