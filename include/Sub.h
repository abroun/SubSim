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

//------------------------------------------------------------------------------
class Sub
{
    public: Sub();
    public: ~Sub();

    public: bool Init( irr::scene::ISceneManager* pSceneManager );
    public: void DeInit();

    private: bool mbInitialised;
    private: irr::scene::ISceneManager* mpSceneManager;
    private: irr::scene::IMesh* mpMesh;
    private: irr::scene::IMeshSceneNode* mpSceneNode;
};

#endif // SUB_H
