//------------------------------------------------------------------------------
// File: Buoy.cpp
// Desc: A floating buoy entity
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "Buoy.h"

//------------------------------------------------------------------------------
Buoy::Buoy()
    : mbInitialised( false ),
    mpMesh( NULL ),
    mpMeshNode( NULL )
{
}

//------------------------------------------------------------------------------
Buoy::~Buoy()
{
    DeInit();
}

//------------------------------------------------------------------------------
bool Buoy::Init( irr::scene::ISceneManager* pSceneManager )
{
    if ( !mbInitialised )
    {
        if ( !Entity::Init( pSceneManager ) )
        {
            DeInit();
            return false;
        }
        
        // Create a sphere for the buoy
        const irr::scene::IGeometryCreator* pCreator = pSceneManager->getGeometryCreator();

        mpMesh = pCreator->createSphereMesh( 1 );
        if ( NULL == mpMesh )
        {
            fprintf( stderr, "Error: Unable to create mesh" );
            DeInit();
            return false;
        }
        
        // Change colour to red
        irr::scene::IMeshBuffer* pMeshBuffer = mpMesh->getMeshBuffer( 0 );
        S32 numVertices = pMeshBuffer->getVertexCount();
        irr::video::S3DVertex* pVertices = (irr::video::S3DVertex*)pMeshBuffer->getVertices();
        for ( S32 vertexIdx = 0; vertexIdx < numVertices; vertexIdx++ )
        {
            pVertices[ vertexIdx ].Color.set( 255, 228, 66, 1 );
        }   
        
        mpMeshNode = pSceneManager->addMeshSceneNode( mpMesh );
        if ( NULL == mpMeshNode )
        {
            fprintf( stderr, "Error: Unable to create cone mesh node" );
            DeInit();
            return false;
        }
        mpMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        
        // Put the node under the control of SubSim
        AddChildNode( mpMeshNode );

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void Buoy::DeInit()
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

