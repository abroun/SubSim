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
    public: void GetPosition( Vector* pPosOut ) const;

    // Accessor for the Yaw of the Sub (rotation around the vertical z-axis)
    // in radians
    public: void SetYaw( F32 yawAngle );
    public: F32 GetYaw() const;

    // Members
    private: bool mbInitialised;
    private: irr::scene::ISceneManager* mpSceneManager;
    private: irr::scene::IMesh* mpMesh;
    private: irr::scene::IMeshSceneNode* mpSceneNode;
};

#endif // SUB_H
