//------------------------------------------------------------------------------
// File: Sub.h
// Desc: The submarine entity
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef SUB_H
#define SUB_H

//------------------------------------------------------------------------------
#include "Entity.h"

//------------------------------------------------------------------------------
class Sub : public Entity
{
    //--------------------------------------------------------------------------
    public: Sub();
    public: ~Sub();

    //--------------------------------------------------------------------------
    public: bool Init( irr::scene::ISceneManager* pSceneManager );
    public: void DeInit();
    
    //--------------------------------------------------------------------------
    // Updates the entity by a given number of milliseconds
    public: virtual void Update( S32 timeStepMS );
    
    //--------------------------------------------------------------------------
    //! Sets the desired forward speed of the submarine in metres per second
    public: void SetForwardSpeed( F32 forwardSpeed ) { mForwardSpeed = forwardSpeed; }
    
    //--------------------------------------------------------------------------
    //! Sets the desired yaw speed of the submarine in radians per second
    public: void SetYawSpeed( F32 yawSpeed ) { mYawSpeed = yawSpeed; }

    //--------------------------------------------------------------------------
    // Members
    private: bool mbInitialised;
    private: irr::scene::IMesh* mpConeMesh;
    private: irr::scene::IMesh* mpBodyMesh;
    private: irr::scene::IMeshSceneNode* mpConeMeshNode;
    private: irr::scene::IMeshSceneNode* mpBodyMeshNode;
    private: F32 mForwardSpeed;
    private: F32 mYawSpeed;
};

#endif // SUB_H
