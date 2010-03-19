//------------------------------------------------------------------------------
// File: Pipe.cpp
// Desc: Represents a UPVC pipe
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "Pipe.h"
#include "Common/MathUtils.h"

//------------------------------------------------------------------------------
const irr::video::SColor Pipe::COLOUR( 255, 255, 255, 0 );
const F32 Pipe::RADIUS = 0.25f;
const F32 Pipe::LENGTH = 25.0f;

//------------------------------------------------------------------------------
Pipe::Pipe()
    : mbInitialised( false ),
    mpMesh( NULL ),
    mpMeshNode( NULL )
{
}

//------------------------------------------------------------------------------
Pipe::~Pipe()
{
    DeInit();
}

//------------------------------------------------------------------------------
bool Pipe::Init( irr::scene::ISceneManager* pSceneManager )
{
    if ( !mbInitialised )
    {
        if ( !Entity::Init( pSceneManager ) )
        {
            DeInit();
            return false;
        }
        
        // Create a cylinder to represent the pipe
        const irr::scene::IGeometryCreator* pCreator = pSceneManager->getGeometryCreator();

        mpMesh = pCreator->createCylinderMesh( RADIUS, LENGTH, 16, COLOUR );
        if ( NULL == mpMesh )
        {
            DeInit();
            return false;
        }
                
        mpMeshNode = pSceneManager->addMeshSceneNode( mpMesh );
        if ( NULL == mpMeshNode )
        {
            DeInit();
            return false;
        }
        mpMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        mpMeshNode->setRotation( irr::core::vector3df( 90.0f, 0.0f, 0.0f ) );
        mpMeshNode->setPosition( irr::core::vector3df( 0.0f, 0.0f, -LENGTH/2.0f ) );
        
        // Put the node under the control of SubSim
        AddChildNode( mpMeshNode );

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void Pipe::DeInit()
{
    RemoveAllChildNodes();
    
    if ( NULL != mpMeshNode )
    {
        mpMeshNode = NULL;
    }
    
    if ( NULL != mpMesh )
    {
        mpMesh->drop();
        mpMesh = NULL;
    }
    
    Entity::DeInit();

    mbInitialised = false;
}

