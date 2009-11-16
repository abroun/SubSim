//------------------------------------------------------------------------------
// File: Gate.h
// Desc: An entity that represents one of the gates the sub must drive through
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef GATE_H
#define GATE_H

//------------------------------------------------------------------------------
#include "Entity.h"

//------------------------------------------------------------------------------
class Gate : public Entity
{
    //--------------------------------------------------------------------------
    public: Gate();
    public: ~Gate();

    //--------------------------------------------------------------------------
    public: bool Init( irr::scene::ISceneManager* pSceneManager );
    public: void DeInit();

    //--------------------------------------------------------------------------
    // Members
    private: bool mbInitialised;
    private: irr::scene::IMesh* mpTopMesh;
    private: irr::scene::IMesh* mpBottomMesh;
    private: irr::scene::IMesh* mpLeftMesh;
    private: irr::scene::IMesh* mpRightMesh;
    private: irr::scene::IMeshSceneNode* mpTopMeshNode;
    private: irr::scene::IMeshSceneNode* mpBottomMeshNode;
    private: irr::scene::IMeshSceneNode* mpLeftMeshNode;
    private: irr::scene::IMeshSceneNode* mpRightMeshNode;
    
    private: static const irr::video::SColor COLOUR;
    private: static const F32 HEIGHT;
    private: static const F32 WIDTH;
    private: static const F32 STRUT_RADIUS;
};

#endif // GATE_H
