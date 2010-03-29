//------------------------------------------------------------------------------
// File: Buoy.h
// Desc: A floating buoy entity
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef BUOY_H
#define BUOY_H

//------------------------------------------------------------------------------
#include "Entity.h"
#include <btBulletDynamicsCommon.h>

//------------------------------------------------------------------------------
class Buoy : public Entity
{
    //--------------------------------------------------------------------------
    public: Buoy();
    public: ~Buoy();
    public: virtual eType GetType() const { return eT_Buoy; }
    
    //--------------------------------------------------------------------------
    public: bool Init( irr::scene::ISceneManager* pSceneManager,
                        btDiscreteDynamicsWorld* pPhysicsWorld, F32 radius = DEFAULT_RADIUS );
    public: void DeInit();
    
    //--------------------------------------------------------------------------
    public: virtual void SetPosition( const Vector& pos );

    //--------------------------------------------------------------------------
    // Members
    private: bool mbInitialised;
    private: irr::scene::IMesh* mpMesh;
    private: irr::scene::IMeshSceneNode* mpMeshNode;
    private: irr::scene::ISceneManager* mpSceneManager;
    private: btDiscreteDynamicsWorld* mpPhysicsWorld; 
    private: btCollisionShape* mpCollisionShape;
    private: btDefaultMotionState* mpMotionState;
    private: btRigidBody* mpPhysicsBody;
    
    public: static const F32 DEFAULT_RADIUS;
    private: static const F32 MASS;
};

#endif // BUOY_H
