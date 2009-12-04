//------------------------------------------------------------------------------
// File: Buoy.cpp
// Desc: A floating buoy entity
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "Buoy.h"
//#include <LinearMath/btTransform.h>

//------------------------------------------------------------------------------
const F32 Buoy::RADIUS = 1.0f;
const F32 Buoy::MASS = 1.0f;

//------------------------------------------------------------------------------
Buoy::Buoy()
    : mbInitialised( false ),
    mpMesh( NULL ),
    mpMeshNode( NULL ),
    mpPhysicsWorld( NULL ),
    mpCollisionShape( NULL ),
    mpMotionState( NULL ),
    mpPhysicsBody( NULL )
{
}

//------------------------------------------------------------------------------
Buoy::~Buoy()
{
    DeInit();
}

//------------------------------------------------------------------------------
bool Buoy::Init( irr::scene::ISceneManager* pSceneManager,
                 btDiscreteDynamicsWorld* pPhysicsWorld )
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

        mpMesh = pCreator->createSphereMesh( RADIUS );
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
        
        // Setup physics representation
        mpPhysicsWorld = pPhysicsWorld;
        mpCollisionShape = new btSphereShape( RADIUS );
        
        btTransform initialTransform;
        initialTransform.setIdentity();

        // A rigid body is dynamic if and only if mass is non zero, 
        // otherwise static
        bool bIsDynamic = ( 0.0f != MASS );
        btVector3 localInertia( 0.0f, 0.0f, 0.0f );
        
        if ( bIsDynamic )
        {
            mpCollisionShape->calculateLocalInertia( MASS, localInertia );
        }
        
        mpMotionState = new btDefaultMotionState( initialTransform );
        mpPhysicsBody = new btRigidBody( 
            btRigidBody::btRigidBodyConstructionInfo( 
                MASS, mpMotionState, mpCollisionShape, localInertia ) );
        
        mpPhysicsWorld->addRigidBody( mpPhysicsBody );

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void Buoy::DeInit()
{ 
    if ( NULL != mpPhysicsBody )
    {
        mpPhysicsWorld->removeCollisionObject( mpPhysicsBody );
        delete mpPhysicsBody;
        mpPhysicsBody = NULL;
    }
    
    if ( NULL != mpMotionState )
    {
        delete mpMotionState;
        mpMotionState = NULL;
    }
    
    if ( NULL != mpCollisionShape )
    {
        delete mpCollisionShape;
        mpCollisionShape = NULL;
    }
    
    mpPhysicsWorld = NULL;
    
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

//------------------------------------------------------------------------------
void Buoy::SetPosition( const Vector& pos )
{
    if ( mbInitialised )
    {
        mpMotionState->m_graphicsWorldTrans.setOrigin( 
            btVector3( pos.mX, pos.mY, pos.mZ ) );
        
        Entity::SetPosition( pos );
    }
}

