//------------------------------------------------------------------------------
// File: FloorTarget.cpp
// Desc: An entity that represents a target on the floor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "FloorTarget.h"
#include "Common/MathUtils.h"

//------------------------------------------------------------------------------
const irr::video::SColor FloorTarget::MAIN_COLOUR( 255, 64, 64, 64 );
const irr::video::SColor FloorTarget::CROSS_COLOUR( 255, 192, 192, 0 );
const F32 FloorTarget::RADIUS = 1.0f;
const F32 FloorTarget::HEIGHT = 0.5f;
const F32 FloorTarget::CROSS_RADIUS = 0.8f;
const F32 FloorTarget::CROSS_WIDTH = 0.1;

//------------------------------------------------------------------------------
FloorTarget::FloorTarget()
    : mbInitialised( false ),
    mpMainMesh( NULL ),
    mpCrossMeshA( NULL ),
    mpCrossMeshB( NULL ),
    mpMainMeshNode( NULL ),
    mpCrossMeshANode( NULL ),
    mpCrossMeshBNode( NULL )
{
}

//------------------------------------------------------------------------------
FloorTarget::~FloorTarget()
{
    DeInit();
}

//------------------------------------------------------------------------------
bool FloorTarget::Init( irr::scene::ISceneManager* pSceneManager )
{
    if ( !mbInitialised )
    {
        if ( !Entity::Init( pSceneManager ) )
        {
            DeInit();
            return false;
        }
        
        // Create a cylinder and cross to represent the target
        const irr::scene::IGeometryCreator* pCreator = pSceneManager->getGeometryCreator();
        
        mpMainMesh = pCreator->createCylinderMesh( RADIUS, HEIGHT, 16, MAIN_COLOUR );
        mpCrossMeshA = pCreator->createCubeMesh( irr::core::vector3df( 2.0*CROSS_RADIUS, HEIGHT, CROSS_WIDTH ) );
        mpCrossMeshB = pCreator->createCubeMesh( irr::core::vector3df( CROSS_WIDTH, HEIGHT, 2.0*CROSS_RADIUS ) );
           if ( NULL == mpMainMesh
            || NULL == mpCrossMeshA || NULL == mpCrossMeshB )
        {
            DeInit();
            return false;
        }
        
        // Change colour of cross
        irr::scene::IMeshBuffer* pMeshBuffer = mpCrossMeshA->getMeshBuffer( 0 );
        S32 numVertices = pMeshBuffer->getVertexCount();
        irr::video::S3DVertex* pVertices = (irr::video::S3DVertex*)pMeshBuffer->getVertices();
        for ( S32 vertexIdx = 0; vertexIdx < numVertices; vertexIdx++ )
        {
            pVertices[ vertexIdx ].Color = CROSS_COLOUR;
        } 
        
        pMeshBuffer = mpCrossMeshB->getMeshBuffer( 0 );
        numVertices = pMeshBuffer->getVertexCount();
        pVertices = (irr::video::S3DVertex*)pMeshBuffer->getVertices();
        for ( S32 vertexIdx = 0; vertexIdx < numVertices; vertexIdx++ )
        {
            pVertices[ vertexIdx ].Color = CROSS_COLOUR;
        } 
        
        mpMainMeshNode = pSceneManager->addMeshSceneNode( mpMainMesh );
        mpCrossMeshANode = pSceneManager->addMeshSceneNode( mpCrossMeshA );
        mpCrossMeshBNode = pSceneManager->addMeshSceneNode( mpCrossMeshB );
        if ( NULL == mpMainMeshNode
            || NULL == mpCrossMeshANode || NULL == mpCrossMeshBNode )
        {
            DeInit();
            return false;
        }
        mpMainMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpMainMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        mpCrossMeshANode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpCrossMeshANode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        mpCrossMeshBNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpCrossMeshBNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
       
        // Position the cross
        mpCrossMeshANode->setPosition( irr::core::vector3df( 0.0f, 0.4f, 0.0f ) );
        mpCrossMeshBNode->setPosition( irr::core::vector3df( 0.0f, 0.4f, 0.0f ) );
                
        // Put the nodes under the control of SubSim
        AddChildNode( mpMainMeshNode );
        AddChildNode( mpCrossMeshANode );
        AddChildNode( mpCrossMeshBNode );

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void FloorTarget::DeInit()
{
    RemoveAllChildNodes();
    
    mpMainMeshNode = NULL;
    mpCrossMeshANode = NULL;
    mpCrossMeshBNode = NULL;
    
    if ( NULL != mpMainMesh )
    {
        mpMainMesh->drop();
        mpMainMesh = NULL;
    }
    
    if ( NULL != mpCrossMeshA )
    {
        mpCrossMeshA->drop();
        mpCrossMeshA = NULL;
    }
    
    if ( NULL != mpCrossMeshB )
    {
        mpCrossMeshB->drop();
        mpCrossMeshB = NULL;
    }
    
    Entity::DeInit();

    mbInitialised = false;
}

