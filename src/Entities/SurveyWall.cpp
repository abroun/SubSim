//------------------------------------------------------------------------------
// File: SurveyWall.cpp
// Desc: Represents the wall that must be surveyed for defects
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "SurveyWall.h"
#include "Common/MathUtils.h"

//------------------------------------------------------------------------------
const irr::video::SColor SurveyWall::COLOUR( 255, 64, 64, 64 );
const F32 SurveyWall::WIDTH = 10.0f;
const F32 SurveyWall::HEIGHT = 5.0f;
const F32 SurveyWall::THICKNESS = 0.5f;

//------------------------------------------------------------------------------
SurveyWall::SurveyWall()
    : mbInitialised( false ),
    mpMesh( NULL ),
    mpMeshNode( NULL )
{
}

//------------------------------------------------------------------------------
SurveyWall::~SurveyWall()
{
    DeInit();
}

//------------------------------------------------------------------------------
bool SurveyWall::Init( irr::scene::ISceneManager* pSceneManager )
{
    if ( !mbInitialised )
    {
        if ( !Entity::Init( pSceneManager ) )
        {
            DeInit();
            return false;
        }
        
        // Create a cube to represent the wall
        const irr::scene::IGeometryCreator* pCreator = pSceneManager->getGeometryCreator();

        mpMesh = pCreator->createCubeMesh( irr::core::vector3df( WIDTH, HEIGHT, THICKNESS ) );
        if ( NULL == mpMesh )
        {
            DeInit();
            return false;
        }
        
        // Colour the mesh
        irr::scene::IMeshBuffer* pMeshBuffer = mpMesh->getMeshBuffer( 0 );
        S32 numVertices = pMeshBuffer->getVertexCount();
        
        irr::video::S3DVertex* pVertices = (irr::video::S3DVertex*)pMeshBuffer->getVertices();
        for ( S32 vertexIdx = 0; vertexIdx < numVertices; vertexIdx++ )
        {
            pVertices[ vertexIdx ].Color = COLOUR;
        }
                
        mpMeshNode = pSceneManager->addMeshSceneNode( mpMesh );
        if ( NULL == mpMeshNode )
        {
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
void SurveyWall::DeInit()
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

