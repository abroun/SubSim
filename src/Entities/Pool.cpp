//------------------------------------------------------------------------------
// File: Pool.cpp
// Desc: Represents a swimming pool
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "Pool.h"
#include "Common/MathUtils.h"

//------------------------------------------------------------------------------
const irr::video::SColor Pool::WALL_COLOUR( 255, 135, 151, 177 );
const F32 Pool::HEIGHT = 100.0f;
const F32 Pool::WIDTH = 200.0f;
const F32 Pool::LENGTH = 400.0f;
const F32 Pool::WALL_THICKNESS = 0.1f;

//------------------------------------------------------------------------------
Pool::Pool()
    : mbInitialised( false ),
    mpBottomMesh( NULL ),
    mpLeftMesh( NULL ),
    mpRightMesh( NULL ),
    mpFrontMesh( NULL ),
    mpBackMesh( NULL ),
    mpBottomMeshNode( NULL ),
    mpLeftMeshNode( NULL ),
    mpRightMeshNode( NULL ),
    mpFrontMeshNode( NULL ),
    mpBackMeshNode( NULL )
{
}

//------------------------------------------------------------------------------
Pool::~Pool()
{
    DeInit();
}

//------------------------------------------------------------------------------
bool Pool::Init( irr::scene::ISceneManager* pSceneManager )
{
    if ( !mbInitialised )
    {
        if ( !Entity::Init( pSceneManager ) )
        {
            DeInit();
            return false;
        }
        
        // Create cuboids to represent the walls
        const irr::scene::IGeometryCreator* pCreator = pSceneManager->getGeometryCreator();
        
        irr::core::vector3df bottomDims( WIDTH, WALL_THICKNESS, LENGTH );
        irr::core::vector3df leftDims( WALL_THICKNESS, HEIGHT, LENGTH );
        irr::core::vector3df rightDims( WALL_THICKNESS, HEIGHT, LENGTH );
        irr::core::vector3df frontDims( WIDTH, HEIGHT, WALL_THICKNESS );
        irr::core::vector3df backDims( WIDTH, HEIGHT, WALL_THICKNESS );
        
        mpBottomMesh = pCreator->createCubeMesh( bottomDims );
        mpLeftMesh = pCreator->createCubeMesh( leftDims );
        mpRightMesh = pCreator->createCubeMesh( rightDims );
        mpFrontMesh = pCreator->createCubeMesh( frontDims );
        mpBackMesh = pCreator->createCubeMesh( backDims );
        if ( NULL == mpBottomMesh 
            || NULL == mpLeftMesh || NULL == mpRightMesh
            || NULL == mpFrontMesh || NULL == mpBackMesh )
        {
            DeInit();
            return false;
        }
        
        // Colour the meshes
        irr::scene::IMesh* pMeshList[] =
        {
            mpBottomMesh,
            mpLeftMesh,
            mpRightMesh,
            mpFrontMesh,
            mpBackMesh
        };
        
        for ( S32 meshIdx = 0; meshIdx < 5; meshIdx++ )
        {
            irr::scene::IMeshBuffer* pMeshBuffer = pMeshList[ meshIdx ]->getMeshBuffer( 0 );
            S32 numVertices = pMeshBuffer->getVertexCount();
            
            irr::video::S3DVertex* pVertices = (irr::video::S3DVertex*)pMeshBuffer->getVertices();
            for ( S32 vertexIdx = 0; vertexIdx < numVertices; vertexIdx++ )
            {
                pVertices[ vertexIdx ].Color = WALL_COLOUR;
            }  
        }
        
        mpBottomMeshNode = pSceneManager->addMeshSceneNode( mpBottomMesh );
        mpLeftMeshNode = pSceneManager->addMeshSceneNode( mpLeftMesh );
        mpRightMeshNode = pSceneManager->addMeshSceneNode( mpRightMesh );
        mpFrontMeshNode = pSceneManager->addMeshSceneNode( mpFrontMesh );
        mpBackMeshNode = pSceneManager->addMeshSceneNode( mpBackMesh );
        if ( NULL == mpBottomMeshNode 
            || NULL == mpLeftMeshNode || NULL == mpRightMeshNode
            || NULL == mpFrontMeshNode || NULL == mpBackMeshNode )
        {
            DeInit();
            return false;
        }
        mpBottomMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpBottomMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        mpLeftMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpLeftMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        mpRightMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpRightMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        mpFrontMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpFrontMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        mpBackMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpBackMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        
        // Position the walls that make up the pool
        mpBottomMeshNode->setPosition( irr::core::vector3df( 0.0f, -HEIGHT / 2.0f, 0.0f ) );
        
        //mpLeftMeshNode->setRotation( irr::core::vector3df( 0.0f, 0.0f, 90.0f ) );
        mpLeftMeshNode->setPosition( irr::core::vector3df( -WIDTH / 2.0f, 0.0f, 0.0f ) );
        //mpRightMeshNode->setRotation( irr::core::vector3df( 0.0f, 0.0f, 90.0f ) );
        mpRightMeshNode->setPosition( irr::core::vector3df( WIDTH / 2.0f, 0.0f, 0.0f ) );
        
        //mpFrontMeshNode->setRotation( irr::core::vector3df( 90.0f, 0.0f, 0.0f ) );
        mpFrontMeshNode->setPosition( irr::core::vector3df( 0.0f, 0.0f, LENGTH / 2.0f ) );
        //mpBackMeshNode->setRotation( irr::core::vector3df( 90.0f, 0.0f, 0.0f ) );
        mpBackMeshNode->setPosition( irr::core::vector3df( 0.0f, 0.0f, -LENGTH / 2.0f ) );
        
        // Put the nodes under the control of SubSim
        AddChildNode( mpBottomMeshNode );
        AddChildNode( mpLeftMeshNode );
        AddChildNode( mpRightMeshNode );
        AddChildNode( mpFrontMeshNode );
        AddChildNode( mpBackMeshNode );

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void Pool::DeInit()
{
    RemoveAllChildNodes();
    
    if ( NULL != mpBottomMeshNode )
    {
        mpBottomMeshNode = NULL;
    }
    
    if ( NULL != mpLeftMeshNode )
    {
        mpLeftMeshNode = NULL;
    }
    
    if ( NULL != mpRightMeshNode )
    {
        mpRightMeshNode = NULL;
    }
    
    if ( NULL != mpFrontMeshNode )
    {
        mpFrontMeshNode = NULL;
    }
    
    if ( NULL != mpBackMeshNode )
    {
        mpBackMeshNode = NULL;
    }
    
    if ( NULL != mpBottomMesh )
    {
        mpBottomMesh->drop();
        mpBottomMesh = NULL;
    }
    
    if ( NULL != mpLeftMesh )
    {
        mpLeftMesh->drop();
        mpLeftMesh = NULL;
    }
    
    if ( NULL != mpRightMesh )
    {
        mpRightMesh->drop();
        mpRightMesh = NULL;
    }
    
    if ( NULL != mpFrontMesh )
    {
        mpFrontMesh->drop();
        mpFrontMesh = NULL;
    }
    
    if ( NULL != mpBackMesh )
    {
        mpBackMesh->drop();
        mpBackMesh = NULL;
    }
    
    Entity::DeInit();

    mbInitialised = false;
}

