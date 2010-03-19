//------------------------------------------------------------------------------
// File: HarbourFloor.h
// Desc: Represents the SAUC-E 2010 harbour
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef HARBOUR_FLOOR_H
#define HARBOUR_FLOOR_H

//------------------------------------------------------------------------------
#include "Entity.h"

//------------------------------------------------------------------------------
class HarbourFloor : public Entity
{
    //--------------------------------------------------------------------------
    public: HarbourFloor();
    public: ~HarbourFloor();
    public: virtual eType GetType() const { return eT_HarbourFloor; }

    //--------------------------------------------------------------------------
    public: bool Init( irr::scene::ISceneManager* pSceneManager );
    public: void DeInit();

    //--------------------------------------------------------------------------
    // Members
    private: bool mbInitialised;
    private: irr::scene::IMesh* mpBottomMesh;
    private: irr::scene::IMeshSceneNode* mpBottomMeshNode;
    
    private: static const irr::video::SColor COLOUR;
    private: static const F32 WIDTH;
    private: static const F32 LENGTH;
    private: static const F32 THICKNESS;
};

#endif // HARBOUR_FLOOR_H
