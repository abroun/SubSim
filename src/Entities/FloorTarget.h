//------------------------------------------------------------------------------
// File: FloorTarget.h
// Desc: An entity that represents a target on the floor
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef FLOOR_TARGET_H
#define FLOOR_TARGET_H

//------------------------------------------------------------------------------
#include "Entity.h"

//------------------------------------------------------------------------------
class FloorTarget : public Entity
{
    //--------------------------------------------------------------------------
    public: FloorTarget();
    public: ~FloorTarget();
    public: virtual eType GetType() const { return eT_FloorTarget; }

    //--------------------------------------------------------------------------
    public: bool Init( irr::scene::ISceneManager* pSceneManager );
    public: void DeInit();

    //--------------------------------------------------------------------------
    // Members
    private: bool mbInitialised;
    private: irr::scene::IMesh* mpMainMesh;
    private: irr::scene::IMesh* mpCrossMeshA;
    private: irr::scene::IMesh* mpCrossMeshB;
    private: irr::scene::IMeshSceneNode* mpMainMeshNode;
    private: irr::scene::IMeshSceneNode* mpCrossMeshANode;
    private: irr::scene::IMeshSceneNode* mpCrossMeshBNode;
    
    private: static const irr::video::SColor MAIN_COLOUR;
    private: static const irr::video::SColor CROSS_COLOUR;
    private: static const F32 RADIUS;
    private: static const F32 HEIGHT;
    private: static const F32 CROSS_RADIUS;
    private: static const F32 CROSS_WIDTH;
};

#endif // FLOOR_TARGET_H
