//------------------------------------------------------------------------------
// File: Sub.cpp
// Desc: The submarine entity
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "Sub.h"
#include "MathUtils.h"

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

        // Create a cylinder for the sub
        const irr::scene::IGeometryCreator* pCreator = mpSceneManager->getGeometryCreator();

        mpMesh = pCreator->createConeMesh( 2, 6, 16, irr::video::SColor( 255, 255, 255, 0 ) );
        if ( NULL == mpMesh )
        {
            fprintf( stderr, "Error: Unable to create mesh" );
            DeInit();
            return false;
        }
        
        mpSceneNode = mpSceneManager->addMeshSceneNode( mpMesh );
        if ( NULL == mpSceneNode )
        {
            fprintf( stderr, "Error: Unable to create scene node" );
            DeInit();
            return false;
        }
        mpSceneNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void Sub::DeInit()
{
    if ( NULL != mpSceneNode )
    {
        mpSceneNode = NULL;
    }

    if ( NULL != mpMesh )
    {
        mpMesh->drop();
        mpMesh = NULL;
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
    }
}

//------------------------------------------------------------------------------
void Sub::GetPosition( Vector* pPosOut ) const
{
    if ( mbInitialised )
    {
    }
}

//------------------------------------------------------------------------------
void Sub::SetYaw( F32 yawAngle )
{
    if ( mbInitialised )
    {
    }
}

//------------------------------------------------------------------------------
F32 Sub::GetYaw() const
{
    F32 yawAngle = 0.0f;

    if ( mbInitialised )
    {
    }

    return yawAngle;
}

