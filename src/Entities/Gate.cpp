//------------------------------------------------------------------------------
// File: Gate.cpp
// Desc: An entity that represents one of the gates the sub must drive through
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "Gate.h"
#include "Common/MathUtils.h"

//------------------------------------------------------------------------------
const irr::video::SColor Gate::COLOUR( 255, 192, 192, 192 );
const F32 Gate::DEFAULT_HEIGHT = 2.5f;
const F32 Gate::DEFAULT_WIDTH = 3.0f;
const F32 Gate::STRUT_RADIUS = 0.05f;

//------------------------------------------------------------------------------
Gate::Gate()
    : mbInitialised( false ),
    mpTopMesh( NULL ),
    mpBottomMesh( NULL ),
    mpLeftMesh( NULL ),
    mpRightMesh( NULL ),
    mpTopMeshNode( NULL ),
    mpBottomMeshNode( NULL ),
    mpLeftMeshNode( NULL ),
    mpRightMeshNode( NULL )
{
}

//------------------------------------------------------------------------------
Gate::~Gate()
{
    DeInit();
}

//------------------------------------------------------------------------------
bool Gate::Init( irr::scene::ISceneManager* pSceneManager, F32 width, F32 height )
{
    if ( !mbInitialised )
    {
        if ( !Entity::Init( pSceneManager ) )
        {
            DeInit();
            return false;
        }
        
        // Create cylinders to represent the gate
        const irr::scene::IGeometryCreator* pCreator = pSceneManager->getGeometryCreator();
        
        mpTopMesh = pCreator->createCylinderMesh( STRUT_RADIUS, width, 16, COLOUR );
        mpBottomMesh = pCreator->createCylinderMesh( STRUT_RADIUS, width, 16, COLOUR );
        mpLeftMesh = pCreator->createCylinderMesh( STRUT_RADIUS, height, 16, COLOUR );
        mpRightMesh = pCreator->createCylinderMesh( STRUT_RADIUS, height, 16, COLOUR );
        if ( NULL == mpTopMesh || NULL == mpBottomMesh 
            || NULL == mpLeftMesh || NULL == mpRightMesh )
        {
            DeInit();
            return false;
        }
        
        mpTopMeshNode = pSceneManager->addMeshSceneNode( mpTopMesh );
        mpBottomMeshNode = pSceneManager->addMeshSceneNode( mpBottomMesh );
        mpLeftMeshNode = pSceneManager->addMeshSceneNode( mpLeftMesh );
        mpRightMeshNode = pSceneManager->addMeshSceneNode( mpRightMesh );
        if ( NULL == mpTopMeshNode || NULL == mpBottomMeshNode 
            || NULL == mpLeftMeshNode || NULL == mpRightMeshNode )
        {
            DeInit();
            return false;
        }
        mpTopMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpTopMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        mpBottomMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpBottomMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        mpLeftMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpLeftMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        mpRightMeshNode->setMaterialFlag( irr::video::EMF_LIGHTING, false );
        mpRightMeshNode->setMaterialFlag( irr::video::EMF_FOG_ENABLE, true );
        
        // Position the struts that make up the gate
        mpTopMeshNode->setRotation( irr::core::vector3df( 0.0f, 0.0f, 90.0f ) );
        mpTopMeshNode->setPosition( irr::core::vector3df( width / 2.0f, height, 0.0f ) );
        mpBottomMeshNode->setRotation( irr::core::vector3df( 0.0f, 0.0f, 90.0f ) );
        mpBottomMeshNode->setPosition( irr::core::vector3df( width / 2.0f, 0.0f, 0.0f ) );
        
        mpLeftMeshNode->setPosition( irr::core::vector3df( -width / 2.0f, 0.0f, 0.0f ) );
        mpRightMeshNode->setPosition( irr::core::vector3df( width / 2.0f, 0.0f, 0.0f ) );
        
        // Put the nodes under the control of SubSim
        AddChildNode( mpTopMeshNode );
        AddChildNode( mpBottomMeshNode );
        AddChildNode( mpLeftMeshNode );
        AddChildNode( mpRightMeshNode );

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void Gate::DeInit()
{
    RemoveAllChildNodes();
    
    if ( NULL != mpTopMeshNode )
    {
        mpTopMeshNode = NULL;
    }
    
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
    
    if ( NULL != mpTopMesh )
    {
        mpTopMesh->drop();
        mpTopMesh = NULL;
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
    
    Entity::DeInit();

    mbInitialised = false;
}

