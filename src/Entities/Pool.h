//------------------------------------------------------------------------------
// File: Pool.h
// Desc: Represents a swimming pool
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef POOL_H
#define POOL_H

//------------------------------------------------------------------------------
#include "Entity.h"

//------------------------------------------------------------------------------
class Pool : public Entity
{
    //--------------------------------------------------------------------------
    public: Pool();
    public: ~Pool();

    //--------------------------------------------------------------------------
    public: bool Init( irr::scene::ISceneManager* pSceneManager );
    public: void DeInit();

    //--------------------------------------------------------------------------
    // Members
    private: bool mbInitialised;
    private: irr::scene::IMesh* mpBottomMesh;
    private: irr::scene::IMesh* mpLeftMesh;
    private: irr::scene::IMesh* mpRightMesh;
    private: irr::scene::IMesh* mpFrontMesh;
    private: irr::scene::IMesh* mpBackMesh;
    private: irr::scene::IMeshSceneNode* mpBottomMeshNode;
    private: irr::scene::IMeshSceneNode* mpLeftMeshNode;
    private: irr::scene::IMeshSceneNode* mpRightMeshNode;
    private: irr::scene::IMeshSceneNode* mpFrontMeshNode;
    private: irr::scene::IMeshSceneNode* mpBackMeshNode;
    
    private: static const irr::video::SColor WALL_COLOUR;
    private: static const F32 HEIGHT;
    private: static const F32 WIDTH;
    private: static const F32 LENGTH;
    private: static const F32 WALL_THICKNESS;
};

#endif // POOL_H
