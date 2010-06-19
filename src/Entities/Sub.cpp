//------------------------------------------------------------------------------
// File: Sub.cpp
// Desc: The submarine entity
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "Sub.h"
#include "Common/MathUtils.h"

const F32 Sub::RADIUS = 0.1f;
const F32 Sub::NOSE_LENGTH = 0.2f;
const F32 Sub::BODY_LENGTH = 0.4f;

//------------------------------------------------------------------------------
Sub::Sub()
    : mbInitialised( false ),
    mpConeMesh( NULL ),
    mpBodyMesh( NULL ),
    mpConeMeshNode( NULL ),
    mpBodyMeshNode( NULL ),
    mpCameraRenderTarget( NULL ),
    mpCameraNode( NULL )
{
}

//------------------------------------------------------------------------------
Sub::~Sub()
{
    DeInit();
}

//------------------------------------------------------------------------------
bool Sub::Init( irr::scene::ISceneManager* pSceneManager,
                 irr::video::IVideoDriver* pVideoDriver )
{
    if ( !mbInitialised )
    {
        if ( !Entity::Init( pSceneManager ) )
        {
            DeInit();
            return false;
        }
        
        printf( "Created base entity\n" );
        
        // Create a cone and cylinder for the sub
        const irr::scene::IGeometryCreator* pCreator = pSceneManager->getGeometryCreator();

        mpConeMesh = pCreator->createConeMesh( RADIUS, NOSE_LENGTH, 16, irr::video::SColor( 255, 255, 255, 0 ) );
        if ( NULL == mpConeMesh )
        {
            fprintf( stderr, "Error: Unable to create cone mesh" );
            DeInit();
            return false;
        }
        
        mpBodyMesh = pCreator->createCylinderMesh( RADIUS, BODY_LENGTH, 16, irr::video::SColor( 255, 255, 192, 0 ) );
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
        mpConeMeshNode->setPosition( irr::core::vector3df( 0.0f, 0.0f, BODY_LENGTH / 2.0f ) );
        mpBodyMeshNode->setRotation( irr::core::vector3df( 90.0f, 0.0f, 0.0f ) );
        mpBodyMeshNode->setPosition( irr::core::vector3df( 0.0f, 0.0f, -BODY_LENGTH / 2.0f ) );
        
        // Put the nodes under the control of SubSim
        AddChildNode( mpConeMeshNode );
        AddChildNode( mpBodyMeshNode );
        
        // Add a camera to the nose of the submarine
        /*if ( pVideoDriver->queryFeature( irr::video::EVDF_RENDER_TO_TARGET ) )
        {
            snprintf( mRenderTargetName, sizeof( mRenderTargetName ), "RTT_%x", *(int*)this );
            mRenderTargetName[ sizeof( mRenderTargetName ) ] = '\0';
            mpCameraRenderTarget = pVideoDriver->addRenderTargetTexture(
                irr::core::dimension2d<U32>(320,240), mRenderTargetName );
                
            mpCameraNode = pSceneManager->addCameraSceneNode(
                0, irr::core::vector3df( 0.0f, 0.0f, 0.0f ), 
                irr::core::vector3df( 0.0f, 0.0f, 1.0f ) );
            
            mpCameraNode->bindTargetAndRotation( true );
            mpCameraNode->setPosition( irr::core::vector3df( 0.0f, 0.0f, BODY_LENGTH / 2.0f ) );
            mpCameraNode->setRotation( irr::core::vector3df( 0.0f, 0.0f, 0.0f ) );
            mpCameraNode->setAspectRatio( 4.0f/3.0f );
            mpCameraNode->setFOV( MathUtils::DegToRad( 44.0f )*3.0f/4.0f );
            mpCameraNode->setNearValue( 0.1f );
                
            // Put the camera node under the control of SubSim
            AddChildNode( mpCameraNode );
        }
        else*/
        {
            fprintf( stderr, "Warning: Render to texture not available. "
                "So the submarine will have no camera\n" );
        }
        
        mForwardSpeed = 0.0f;
        mDepthSpeed = 0.0f;
        mYawSpeed = 0.0f;
        mPitchSpeed = 0.0f;

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void Sub::DeInit()
{
    mpCameraRenderTarget = NULL;
    mpCameraNode = NULL;
    
    RemoveAllChildNodes();
    
    mpConeMeshNode = NULL;
    if ( NULL != mpConeMesh )
    {
        mpConeMesh->drop();
        mpConeMesh = NULL;
    }
    
    mpBodyMeshNode = NULL;
    if ( NULL != mpBodyMesh )
    {
        mpBodyMesh->drop();
        mpBodyMesh = NULL;
    }
    
    Entity::DeInit();

    mbInitialised = false;
}

//------------------------------------------------------------------------------
void Sub::Update( F32 timeStep )
{
    // For now we assume that the motion is linear and that forward motion
    // happens before changes in heading
    
    // Update the position of the sub
    // forward is y, positive z is up
    //    y
    //    |_ x
    //   /
    //  z
    F32 oldYaw = GetYaw();
    F32 oldPitch = GetPitch();
    Vector heading( -(F32)sin( oldYaw ), (F32)cos( oldYaw ), (F32)sin( oldPitch ) );
    Vector newPos = GetPosition() + (heading * mForwardSpeed * timeStep) + Vector( 0.0f, 0.0f, mDepthSpeed*timeStep ) ;
    
    F32 newYaw = oldYaw + (mYawSpeed * timeStep);
    F32 newPitch = oldPitch + (mPitchSpeed * timeStep);
    
    SetPosition( newPos );
    SetRotation( mRotation );
    SetYaw( newYaw );
    SetPitch( newPitch );
    
    // pitch angle
    // Vector newRotation = mRotation;
    // newRotation.mX = MathUtils::DegToRad( 120.0f );
    
    
    if ( NULL != mpCameraNode )
    {
        Vector newHeading( -(F32)sin( newYaw ), (F32)cos( newYaw ), (F32)sin( newPitch ) );
        Vector camTarget = newPos + 10.0f*newHeading;
        irr::core::vector3df irrCamTarget = MathUtils::TransformVector_SubToIrr( camTarget );
        mpCameraNode->setTarget( irrCamTarget );
    }
}

