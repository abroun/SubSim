//------------------------------------------------------------------------------
// File: Entity.h
// Desc: A good old fashioned base entity class
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef ENTITY_H
#define ENTITY_H

//------------------------------------------------------------------------------
#include <irrlicht/irrlicht.h>
#include "Common.h"
#include "Vector.h"

//------------------------------------------------------------------------------
class Entity
{
    //--------------------------------------------------------------------------
    public: Entity();
    public: virtual ~Entity();

    //--------------------------------------------------------------------------
    public: virtual bool Init( irr::scene::ISceneManager* pSceneManager );
    public: virtual void DeInit();

    //--------------------------------------------------------------------------
    public: virtual void SetPosition( const Vector& pos );
    public: virtual const Vector& GetPosition() const;
    
    //--------------------------------------------------------------------------
    // Rotations are given in radians
    public: virtual void SetRotation( const Vector& rotation );
    public: virtual const Vector& GetRotation() const;

    //--------------------------------------------------------------------------
    // Accessor for the Yaw of the entity (rotation around the vertical z-axis)
    // in radians
    public: void SetYaw( F32 yawAngle );
    public: F32 GetYaw() const;
    
    //--------------------------------------------------------------------------
    // Helper routine for working with the entity's transform
    private: void UpdateTransform();
    
    //--------------------------------------------------------------------------
    public: irr::scene::ISceneManager* GetSceneManager() const { return mpSceneManager; }
    
    //--------------------------------------------------------------------------
    // Interface for adding and removing nodes that will be transformed with
    // the entity transform
    protected: void AddChildNode( irr::scene::ISceneNode* pChildNode );
    protected: void RemoveChildNode( irr::scene::ISceneNode* pChildNode );
    protected: void RemoveAllChildNodes();
    
    //--------------------------------------------------------------------------
    // Updates the entity by a given number of seconds
    public: virtual void Update( F32 timeStep ) {}

    //--------------------------------------------------------------------------
    // Members
    private: bool mbInitialised;
    private: irr::scene::ISceneManager* mpSceneManager;
    // Node for SubSim transforms
    private: irr::scene::IDummyTransformationSceneNode* mpTransformNode;
    
    // Translation and rotation in SubSim coordinates
    private: Vector mTranslation;
    private: Vector mRotation;
};

#endif // ENTITY_H