//------------------------------------------------------------------------------
// File: CircularPool.h
// Desc: Represents a circular pool, like the test one in the BRL
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "CircularPool.h"
#include "Common/MathUtils.h"

//------------------------------------------------------------------------------
const irr::video::SColor CircularPool::WALL_COLOUR( 255, 0, 0, 0 );
const irr::video::SColor CircularPool::FLOOR_COLOUR( 255, 64, 64, 64 );
const F32 CircularPool::HEIGHT = 1.0f;
const F32 CircularPool::DEFAULT_RADIUS = 2.0f;
const F32 CircularPool::WALL_THICKNESS = 0.1f;

//------------------------------------------------------------------------------
CircularPool::CircularPool()
    : mbInitialised( false ),
    mpBottomMesh( NULL ),
    mpWallMesh( NULL ),
    mpBottomMeshNode( NULL ),
    mpWallMeshNode( NULL )
{
}

//------------------------------------------------------------------------------
CircularPool::~CircularPool()
{
    DeInit();
}

//------------------------------------------------------------------------------
bool CircularPool::Init( irr::scene::ISceneManager* pSceneManager, F32 radius )
{
    if ( !mbInitialised )
    {
        if ( !Entity::Init( pSceneManager ) )
        {
            DeInit();
            return false;
        }
        
        // Create geometry to represent the pool
        const irr::scene::IGeometryCreator* pCreator = pSceneManager->getGeometryCreator();
        
        mpBottomMesh = pCreator->createCylinderMesh( radius, WALL_THICKNESS, 16, FLOOR_COLOUR, true );
        mpWallMesh = pCreator->createCylinderMesh( radius, HEIGHT, 16, WALL_COLOUR, false );
        
        if ( NULL == mpBottomMesh 
            || NULL == mpWallMesh )
        {
            DeInit();
            return false;
        }
        
        mpBottomMeshNode = pSceneManager->addMeshSceneNode( mpBottomMesh );
        mpWallMeshNode = pSceneManager->addMeshSceneNode( mpWallMesh );
        if ( NULL == mpBottomMeshNode 
            || NULL == mpWallMeshNode )
        {
            DeInit();
            return false;
        }
        mpBottomMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpBottomMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        mpWallMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpWallMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        mpWallMeshNode->setMaterialFlag( irr::video::EMF_BACK_FACE_CULLING, false );

        // Position the floor
        mpBottomMeshNode->setPosition( irr::core::vector3df( 0.0f, -WALL_THICKNESS / 2.0f, 0.0f ) );
        
        // Put the nodes under the control of SubSim
        AddChildNode( mpBottomMeshNode );
        AddChildNode( mpWallMeshNode );

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void CircularPool::DeInit()
{
    RemoveAllChildNodes();
    
    if ( NULL != mpBottomMeshNode )
    {
        mpBottomMeshNode = NULL;
    }
    
    if ( NULL != mpWallMeshNode )
    {
        mpWallMeshNode = NULL;
    }
    
    if ( NULL != mpBottomMesh )
    {
        mpBottomMesh->drop();
        mpBottomMesh = NULL;
    }
    
    if ( NULL != mpWallMesh )
    {
        mpWallMesh->drop();
        mpWallMesh = NULL;
    }
    
    Entity::DeInit();

    mbInitialised = false;
}

