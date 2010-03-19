//------------------------------------------------------------------------------
// File: HarbourFloor.cpp
// Desc: Represents the SAUC-E 2010 harbour
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "HarbourFloor.h"
#include "Common/MathUtils.h"

//------------------------------------------------------------------------------
const irr::video::SColor HarbourFloor::COLOUR( 255, 135, 151, 177 );
const F32 HarbourFloor::WIDTH = 60.0f;
const F32 HarbourFloor::LENGTH = 25.0f;
const F32 HarbourFloor::THICKNESS = 0.1f;

//------------------------------------------------------------------------------
HarbourFloor::HarbourFloor()
    : mbInitialised( false ),
    mpBottomMesh( NULL ),
    mpBottomMeshNode( NULL )
{
}

//------------------------------------------------------------------------------
HarbourFloor::~HarbourFloor()
{
    DeInit();
}

//------------------------------------------------------------------------------
bool HarbourFloor::Init( irr::scene::ISceneManager* pSceneManager )
{
    if ( !mbInitialised )
    {
        if ( !Entity::Init( pSceneManager ) )
        {
            DeInit();
            return false;
        }
        
        // Create a cuboid to represent the floor
        const irr::scene::IGeometryCreator* pCreator = pSceneManager->getGeometryCreator();
        
        irr::core::vector3df bottomDims( WIDTH, THICKNESS, LENGTH );
        
        mpBottomMesh = pCreator->createCubeMesh( bottomDims );
        if ( NULL == mpBottomMesh )
        {
            DeInit();
            return false;
        }
        
        // Colour the mesh
        irr::scene::IMeshBuffer* pMeshBuffer = mpBottomMesh->getMeshBuffer( 0 );
        S32 numVertices = pMeshBuffer->getVertexCount();
        
        irr::video::S3DVertex* pVertices = (irr::video::S3DVertex*)pMeshBuffer->getVertices();
        for ( S32 vertexIdx = 0; vertexIdx < numVertices; vertexIdx++ )
        {
            pVertices[ vertexIdx ].Color = COLOUR;
        }  
                
        mpBottomMeshNode = pSceneManager->addMeshSceneNode( mpBottomMesh );
        if ( NULL == mpBottomMeshNode )
        {
            DeInit();
            return false;
        }
        mpBottomMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpBottomMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        
        // Position the floor
        mpBottomMeshNode->setPosition( irr::core::vector3df( 0.0f, -THICKNESS/2.0f, 0.0f ) );
       
        // Put the nodes under the control of SubSim
        AddChildNode( mpBottomMeshNode );

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void HarbourFloor::DeInit()
{
    RemoveAllChildNodes();
    
    if ( NULL != mpBottomMeshNode )
    {
        mpBottomMeshNode = NULL;
    }
    
    if ( NULL != mpBottomMesh )
    {
        mpBottomMesh->drop();
        mpBottomMesh = NULL;
    }
    
    Entity::DeInit();

    mbInitialised = false;
}

