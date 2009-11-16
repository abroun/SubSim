//------------------------------------------------------------------------------
// File: Sub.h
// Desc: The submarine entity
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef SUB_H
#define SUB_H

//------------------------------------------------------------------------------
#include <irrlicht/irrlicht.h>
#include "Common.h"
#include "Vector.h"

//------------------------------------------------------------------------------
class Sub
{
    public: Sub();
    public: ~Sub();

    public: bool Init( irr::scene::ISceneManager* pSceneManager );
    public: void DeInit();

    // Sub position accessor
    public: void SetPosition( const Vector& pos );
    public: const Vector& GetPosition() const;

    // Accessor for the Yaw of the Sub (rotation around the vertical z-axis)
    // in radians
    public: void SetYaw( F32 yawAngle );
    public: F32 GetYaw() const;
    
    // Helper routine for working with the sub's transform
    private: void UpdateTransform();

    // Members
    private: bool mbInitialised;
    private: irr::scene::ISceneManager* mpSceneManager;
    private: irr::scene::IMesh* mpConeMesh;
    private: irr::scene::IMesh* mpBodyMesh;
    private: irr::scene::IMeshSceneNode* mpConeMeshNode;
    private: irr::scene::IMeshSceneNode* mpBodyMeshNode;
    // Seperate node to keep sub transforms seperate from those used to
    // get the sub model into the correct initial position
    private: irr::scene::IDummyTransformationSceneNode* mpTransformNode;
    
    // Translation and rotation in SubSim coordinates
    private: Vector mTranslation;
    private: Vector mRotation;
};

#endif // SUB_H
