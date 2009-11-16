//------------------------------------------------------------------------------
// File: Sub.cpp
// Desc: The submarine entity
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "Sub.h"
#include "Common/MathUtils.h"

//------------------------------------------------------------------------------
Sub::Sub()
    : mbInitialised( false )
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
        mpSceneManager = pSceneManager;
        mpSceneManager->grab();

        // Create a cone and cylinder for the sub
        const irr::scene::IGeometryCreator* pCreator = mpSceneManager->getGeometryCreator();

        mpConeMesh = pCreator->createConeMesh( 2, 6, 16, irr::video::SColor( 255, 255, 255, 0 ) );
        if ( NULL == mpConeMesh )
        {
            fprintf( stderr, "Error: Unable to create cone mesh" );
            DeInit();
            return false;
        }
        
        mpBodyMesh = pCreator->createCylinderMesh( 2, 3, 16, irr::video::SColor( 255, 255, 0, 0 ) );
        if ( NULL == mpBodyMesh )
        {
            fprintf( stderr, "Error: Unable to create body mesh" );
            DeInit();
            return false;
        }
        
        mpConeMeshNode = mpSceneManager->addMeshSceneNode( mpConeMesh );
        if ( NULL == mpConeMeshNode )
        {
            fprintf( stderr, "Error: Unable to create cone mesh node" );
            DeInit();
            return false;
        }
        mpConeMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        
        mpBodyMeshNode = mpSceneManager->addMeshSceneNode( mpBodyMesh );
        if ( NULL == mpBodyMeshNode )
        {
            fprintf( stderr, "Error: Unable to create body mesh node" );
            DeInit();
            return false;
        }
        mpBodyMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        
        // Correctly position the sub model
        mpConeMeshNode->setRotation( irr::core::vector3df( 90.0f, 0.0f, 0.0f ) );
        mpConeMeshNode->setPosition( irr::core::vector3df( 0.0f, 0.0f, 3.0f ) );
        mpBodyMeshNode->setRotation( irr::core::vector3df( 90.0f, 0.0f, 0.0f ) );
        
        // Now create a parent node to hold any further sub transformations
        mpTransformNode = mpSceneManager->addDummyTransformationSceneNode();
        if ( NULL == mpTransformNode )
        {
            DeInit();
            return false;
        }

        mpTransformNode->addChild( mpConeMeshNode );
        mpTransformNode->addChild( mpBodyMeshNode );
        
        mTranslation.Set( 0.0f, 0.0f, 0.0f );
        mRotation.Set( 0.0f, 0.0f, 0.0f );
        UpdateTransform();

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void Sub::DeInit()
{
    if ( NULL != mpTransformNode )
    {
        mpTransformNode = NULL;
    }
    
    if ( NULL != mpConeMeshNode )
    {
        mpConeMeshNode = NULL;
    }

    if ( NULL != mpConeMesh )
    {
        mpConeMesh->drop();
        mpConeMesh = NULL;
    }

    if ( NULL != mpSceneManager )
    {
        mpSceneManager->drop();
        mpSceneManager = NULL;
    }

    mbInitialised = false;
}

//------------------------------------------------------------------------------
void Sub::SetPosition( const Vector& pos )
{
    if ( mbInitialised )
    {
        mTranslation = pos;
        UpdateTransform();
    }
}

//------------------------------------------------------------------------------
const Vector& Sub::GetPosition() const
{
    return mTranslation;
}

//------------------------------------------------------------------------------
void Sub::SetYaw( F32 yawAngle )
{
    if ( mbInitialised )
    {
        mRotation.mZ = yawAngle;
        UpdateTransform();
    }
}

//------------------------------------------------------------------------------
F32 Sub::GetYaw() const
{
    return mRotation.mZ;
}

//------------------------------------------------------------------------------
void Sub::UpdateTransform()
{
    irr::core::vector3df irrRotation = MathUtils::TransformRotation_SubToIrr( mRotation );
    irr::core::vector3df irrTranslation = MathUtils::TransformVector_SubToIrr( mTranslation );
    
    irr::core::matrix4& subTransform = mpTransformNode->getRelativeTransformationMatrix();
    subTransform.setRotationRadians( irrRotation );
    subTransform.setTranslation( irrTranslation );
}
