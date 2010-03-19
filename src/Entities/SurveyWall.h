//------------------------------------------------------------------------------
// File: SurveyWall.h
// Desc: Represents the wall that must be surveyed for defects
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef SURVEY_WALL_H
#define SURVEY_WALL_H

//------------------------------------------------------------------------------
#include "Entity.h"

//------------------------------------------------------------------------------
class SurveyWall : public Entity
{
    //--------------------------------------------------------------------------
    public: SurveyWall();
    public: ~SurveyWall();
    public: virtual eType GetType() const { return eT_SurveyWall; }

    //--------------------------------------------------------------------------
    public: bool Init( irr::scene::ISceneManager* pSceneManager );
    public: void DeInit();

    //--------------------------------------------------------------------------
    // Members
    private: bool mbInitialised;
    private: irr::scene::IMesh* mpMesh;
    private: irr::scene::IMeshSceneNode* mpMeshNode;
    
    private: static const irr::video::SColor COLOUR;
    private: static const F32 WIDTH;
    private: static const F32 HEIGHT;
    private: static const F32 THICKNESS;
};

#endif // SURVEY_WALL_H
