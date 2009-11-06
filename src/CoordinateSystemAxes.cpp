//------------------------------------------------------------------------------
// File: CoordinateSystemAxes.cpp
// Desc: A 3d object that draws a set of axes for the simulator coordinate
//       system.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "CoordinateSystemAxes.h"

//------------------------------------------------------------------------------
CoordinateSystemAxes::CoordinateSystemAxes()
    : mbInitialised( false )
{
}

//------------------------------------------------------------------------------
CoordinateSystemAxes::~CoordinateSystemAxes()
{
    DeInit();
}

//------------------------------------------------------------------------------
bool CoordinateSystemAxes::Init( irr::scene::ISceneManager* pSceneManager )
{
    if ( !mbInitialised )
    {
        mpSceneManager = pSceneManager;
        mpSceneManager->grab();

        // Create arrows to make up the axes
        const irr::scene::IGeometryCreator* pCreator = mpSceneManager->getGeometryCreator();
    
        if ( !CreateArrow( pCreator, irr::video::SColor( 255, 0, 0, 255 ), 
            &mpArrowMeshX, &mpArrowNodeX ) )
        {
            DeInit();
            return false;
        }

        if ( !CreateArrow( pCreator, irr::video::SColor( 255, 0, 255, 0 ),
            &mpArrowMeshY, &mpArrowNodeY ) )
        {
            DeInit();
            return false;
        }

        if ( !CreateArrow( pCreator, irr::video::SColor( 255, 255, 0, 0 ),
            &mpArrowMeshZ, &mpArrowNodeZ ) )
        {
            DeInit();
            return false;
        }

        // Position the arrows to match the simulator coordinate system
        mpArrowNodeX->setRotation( irr::core::vector3df( 0.0f, 0.0f, -90.0f ) );
        mpArrowNodeY->setRotation( irr::core::vector3df( 90.0f, 0.0f, 0.0f ) );
        mpArrowNodeZ->setRotation( irr::core::vector3df( 0.0f, 0.0f, 0.0f ) );

        // Create a root scene node to hold the arrows
        mpRootSceneNode = mpSceneManager->addDummyTransformationSceneNode();
        if ( NULL == mpRootSceneNode )
        {
            DeInit();
            return false;
        }

        mpRootSceneNode->addChild( mpArrowNodeX );
        mpRootSceneNode->addChild( mpArrowNodeY );
        mpRootSceneNode->addChild( mpArrowNodeZ );

        irr::core::matrix4& rootTransform = mpRootSceneNode->getRelativeTransformationMatrix();
        rootTransform.makeIdentity();
        rootTransform.setTranslation( irr::core::vector3d<F32>( 4.0f, 0.0f, 0.0f ) );

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void CoordinateSystemAxes::DeInit()
{
    if ( NULL != mpRootSceneNode )
    {
        mpRootSceneNode = NULL;
    }

    // Get rid of arrow scene nodes
    if ( NULL != mpArrowNodeX )
    {
        mpArrowNodeX = NULL;
    }

    if ( NULL != mpArrowNodeY )
    {
        mpArrowNodeY = NULL;
    }

    if ( NULL != mpArrowNodeZ )
    {
        mpArrowNodeZ = NULL;
    }

    // Get rid of arrow meshes
    if ( NULL != mpArrowMeshX )
    {
        mpArrowMeshX->drop();
        mpArrowMeshX = NULL;
    }

    if ( NULL != mpArrowMeshY )
    {
        mpArrowMeshY->drop();
        mpArrowMeshY = NULL;
    }

    if ( NULL != mpArrowMeshZ )
    {
        mpArrowMeshZ->drop();
        mpArrowMeshZ = NULL;
    }

    if ( NULL != mpSceneManager )
    {
        mpSceneManager->drop();
        mpSceneManager = NULL;
    }

    mbInitialised = false;
}

//------------------------------------------------------------------------------
bool CoordinateSystemAxes::CreateArrow( 
        const irr::scene::IGeometryCreator* pCreator,
        const irr::video::SColor& colour,
        irr::scene::IMesh** ppArrowMeshOut, 
        irr::scene::IMeshSceneNode** ppArrowNodeOut )
{
    *ppArrowMeshOut = pCreator->createArrowMesh( 
        4, 8, 1.0f, 0.6f, 0.05f, 0.1f, colour, colour );
    if ( NULL == *ppArrowMeshOut )
    {
        return false;
    } 
    
    *ppArrowNodeOut = mpSceneManager->addMeshSceneNode( *ppArrowMeshOut );
    if ( NULL == *ppArrowMeshOut )
    {
        return false;
    }
    (*ppArrowNodeOut)->setMaterialFlag( irr::video::EMF_LIGHTING, false );

    return true;
}


