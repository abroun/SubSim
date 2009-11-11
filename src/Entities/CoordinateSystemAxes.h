//------------------------------------------------------------------------------
// File: CoordinateSystemAxes.h
// Desc: A 3d object that draws a set of axes for the simulator coordinate
//       system.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef COORDINATE_SYSTEM_AXES_H
#define COORDINATE_SYSTEM_AXES_H

//------------------------------------------------------------------------------
#include <irrlicht/irrlicht.h>
#include "Common.h"

//------------------------------------------------------------------------------
class CoordinateSystemAxes
{
    public: CoordinateSystemAxes();
    public: ~CoordinateSystemAxes();

    public: bool Init( irr::scene::ISceneManager* pSceneManager );
    public: void DeInit();

    // Helper routine to create an arrow and a scene node to hold it for an 
    // axis. Returns false if unable to create it.
    private: bool CreateArrow( 
        const irr::scene::IGeometryCreator* pCreator,
        const irr::video::SColor& colour,
        irr::scene::IMesh** ppArrowMeshOut,     
        irr::scene::IMeshSceneNode** ppArrowNodeOut );

    // Member variables
    private: bool mbInitialised;
    private: irr::scene::ISceneManager* mpSceneManager;
    private: irr::scene::IMesh* mpArrowMeshX;
    private: irr::scene::IMesh* mpArrowMeshY;
    private: irr::scene::IMesh* mpArrowMeshZ;
    private: irr::scene::IMeshSceneNode* mpArrowNodeX;
    private: irr::scene::IMeshSceneNode* mpArrowNodeY;
    private: irr::scene::IMeshSceneNode* mpArrowNodeZ;
    private: irr::scene::IDummyTransformationSceneNode* mpRootSceneNode;
};

#endif // COORDINATE_SYSTEM_AXES_H
