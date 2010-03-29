//------------------------------------------------------------------------------
// File: CircularPool.h
// Desc: Represents a circular pool, like the test one in the BRL
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef CIRCULAR_POOL_H
#define CIRCULAR_POOL_H

//------------------------------------------------------------------------------
#include "Entity.h"

//------------------------------------------------------------------------------
class CircularPool : public Entity
{
    //--------------------------------------------------------------------------
    public: CircularPool();
    public: ~CircularPool();
    public: virtual eType GetType() const { return eT_CircularPool; }

    //--------------------------------------------------------------------------
    public: bool Init( irr::scene::ISceneManager* pSceneManager, F32 radius = DEFAULT_RADIUS );
    public: void DeInit();

    //--------------------------------------------------------------------------
    // Members
    private: bool mbInitialised;
    private: irr::scene::IMesh* mpBottomMesh;
    private: irr::scene::IMesh* mpWallMesh;
    private: irr::scene::IMeshSceneNode* mpBottomMeshNode;
    private: irr::scene::IMeshSceneNode* mpWallMeshNode;
    
    private: static const irr::video::SColor WALL_COLOUR;
    private: static const irr::video::SColor FLOOR_COLOUR;
    private: static const F32 HEIGHT;
    public: static const F32 DEFAULT_RADIUS;
    private: static const F32 WALL_THICKNESS;
};

#endif // CIRCULAR_POOL_H
