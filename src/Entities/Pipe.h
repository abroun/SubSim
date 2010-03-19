//------------------------------------------------------------------------------
// File: Pipe.h
// Desc: Represents a UPVC pipe
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef PIPE_H
#define PIPE_H

//------------------------------------------------------------------------------
#include "Entity.h"

//------------------------------------------------------------------------------
class Pipe : public Entity
{
    //--------------------------------------------------------------------------
    public: Pipe();
    public: ~Pipe();
    public: virtual eType GetType() const { return eT_Pipe; }

    //--------------------------------------------------------------------------
    public: bool Init( irr::scene::ISceneManager* pSceneManager );
    public: void DeInit();

    //--------------------------------------------------------------------------
    // Members
    private: bool mbInitialised;
    private: irr::scene::IMesh* mpMesh;
    private: irr::scene::IMeshSceneNode* mpMeshNode;
    
    private: static const irr::video::SColor COLOUR;
    private: static const F32 RADIUS;
    private: static const F32 LENGTH;
};

#endif // PIPE_H
