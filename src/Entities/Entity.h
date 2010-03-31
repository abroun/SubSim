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
    public: enum eType
    {
        eT_Invalid = -1,
        eT_Sub = 0,
        eT_Buoy,
        eT_CoordinateSystemAxes,
        eT_FloorTarget,
        eT_Gate,
        eT_Pool,
        eT_CircularPool,    // TODO: Combine the Pool and CircularPool in some way
        eT_HarbourFloor,
        eT_Pipe,
        eT_SurveyWall,
        
        eT_NumTypes
    };
    
    //--------------------------------------------------------------------------
    public: static const char* TYPE_NAMES[ eT_NumTypes ];
    
    //--------------------------------------------------------------------------
    // Returns eT_InvalidType if the type cannot be found
    public: static eType GetTypeFromString( const char* pTypeString );
    // Returns NULL if the given type is invalid
    public: static const char* ConvertTypeToString( eType type );
    
    //--------------------------------------------------------------------------
    public: Entity();
    public: virtual ~Entity();
    public: virtual eType GetType() const = 0;

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
    public: void SetName( const char* name );
    public: const char* GetName() const { return mName; }

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
    protected: Vector mTranslation;
    protected: Vector mRotation;
    
    public: static const S32 MAX_NAME_LENGTH = 31;
    private: char mName[ MAX_NAME_LENGTH + 1 ];
    private: static S32 mEntityCount;
};

#endif // ENTITY_H