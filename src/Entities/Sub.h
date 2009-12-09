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
    public: bool Init( irr::scene::ISceneManager* pSceneManager, 
                        irr::video::IVideoDriver* pVideoDriver );
    public: void DeInit();
    
    //--------------------------------------------------------------------------
    // Updates the entity by a given number of seconds
    public: virtual void Update( F32 timeStep );
    
    //--------------------------------------------------------------------------
    //! Sets the desired forward speed of the submarine in metres per second
    public: void SetForwardSpeed( F32 forwardSpeed ) { mForwardSpeed = forwardSpeed; }
    
    //--------------------------------------------------------------------------
    //! Sets the desired yaw speed of the submarine in radians per second
    public: void SetYawSpeed( F32 yawSpeed ) { mYawSpeed = yawSpeed; }

    //--------------------------------------------------------------------------
    //! Gets the render target that the simulator will use to render the view
    //! from the submarines camera
    public: irr::video::ITexture* GetCameraRenderTarget() const { return mpCameraRenderTarget; }

    //--------------------------------------------------------------------------
    public: irr::scene::ICameraSceneNode* GetCameraNode() const { return mpCameraNode; }

    //--------------------------------------------------------------------------
    // Members
    private: bool mbInitialised;
    private: irr::scene::IMesh* mpConeMesh;
    private: irr::scene::IMesh* mpBodyMesh;
    private: irr::scene::IMeshSceneNode* mpConeMeshNode;
    private: irr::scene::IMeshSceneNode* mpBodyMeshNode;
    private: F32 mForwardSpeed;
    private: F32 mYawSpeed;
    private: irr::video::ITexture* mpCameraRenderTarget;
    private: irr::scene::ICameraSceneNode* mpCameraNode;
    
    private: static const F32 RADIUS;
    private: static const F32 NOSE_LENGTH;
    private: static const F32 BODY_LENGTH;
};

#endif // SUB_H
