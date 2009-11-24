//------------------------------------------------------------------------------
// File: Simulator.cpp
// Desc: The main simulator class
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "Simulator/Simulator.h"

#include <stdio.h>
#include <time.h>
#include <vector>
#include <irrlicht/irrlicht.h>

#include "Common.h"
#include "Common/MathUtils.h"
#include "Common/HighPrecisionTime.h"
#include "Entities/Sub.h"
#include "Entities/CoordinateSystemAxes.h"
#include "Entities/Gate.h"
#include "Entities/Buoy.h"
#include "Entities/Pool.h"

//------------------------------------------------------------------------------
// Constants and Typdefs
//------------------------------------------------------------------------------
static F32 SIM_DESIRED_SIM_FPS = 30.0f;
static S32 SIM_MICRO_SECS_PER_SIM_FRAME = (S32)(1000000.0f / SIM_DESIRED_SIM_FPS);
static F32 SIM_SECS_PER_SIM_FRAME = 1.0f / SIM_DESIRED_SIM_FPS;
static S32 SIM_MAX_NUM_CATCHUP_FRAMES = 30; // If the simulator gets more than
                                            // this number of frames behind it
                                            // will start dropping frames

typedef std::vector<Entity*> EntityPtrVector;

//------------------------------------------------------------------------------
// SimulatorImpl
//------------------------------------------------------------------------------
struct SimulatorImpl
{
    bool mbInitialised;
    irr::IrrlichtDevice* mpIrrDevice;    
    irr::gui::IGUIStaticText* mpText;
    irr::scene::ICameraSceneNode* mpCamera;
    
    Sub mSub;
    CoordinateSystemAxes mAxes;
    Gate mGate;
    Buoy mBuoy;
    Pool mPool;
    EntityPtrVector mEntityList;
    
    HighPrecisionTime mLastTime;
    S32 mTimeAccumulatorUS; // The number of microseconds that we need to deal with in the next update
    
    S32 mLastFPS;
    bool mbIsRunning;
    
    // Render to texture test
    irr::scene::ISceneNode* mpTestCube;
    irr::video::ITexture* mpRenderTarget;
    irr::scene::ICameraSceneNode* mpFixedCam;
};

//------------------------------------------------------------------------------
// Simulator
//------------------------------------------------------------------------------
Simulator::Simulator()
    : mpImpl( new SimulatorImpl() )
{
    mpImpl->mbInitialised = false;
    mpImpl->mpIrrDevice = NULL;    
    mpImpl->mpText = NULL;
    mpImpl->mpCamera = NULL;
    
    mpImpl->mbIsRunning = false;
}

//------------------------------------------------------------------------------
Simulator::~Simulator()
{
    DeInit();
    delete mpImpl;
    mpImpl = NULL;
}

//------------------------------------------------------------------------------
bool Simulator::Init()
{
    if ( !mpImpl->mbInitialised )
    {
        mpImpl->mpIrrDevice = irr::createDevice( 
            irr::video::EDT_OPENGL, 
            irr::core::dimension2d<irr::u32>( 640, 480 ), 
            16, false, false, false, 0 );

        if ( NULL == mpImpl->mpIrrDevice )
        {
            fprintf( stderr, "Error: Unable to create Irrlicht Device\n" );
            DeInit();
            return false;
        }

        mpImpl->mpIrrDevice->setWindowCaption( L"SubSim" );
    
        irr::video::IVideoDriver* pVideoDriver = mpImpl->mpIrrDevice->getVideoDriver();
        irr::scene::ISceneManager* pSceneMgr = mpImpl->mpIrrDevice->getSceneManager();
        irr::gui::IGUIEnvironment* pGUIEnvironment = mpImpl->mpIrrDevice->getGUIEnvironment();
    
        mpImpl->mpText = pGUIEnvironment->addStaticText( 
            L"Hello World!", irr::core::rect< irr::s32 >( 10, 10, 200, 40 ), true );
    
        // Setup sub
        if ( !mpImpl->mSub.Init( pSceneMgr ) )
        {
            fprintf( stderr, "Error: Unable to initialise sub\n" );
            DeInit();
            return false;
        }
        mpImpl->mSub.SetYaw( MathUtils::DegToRad( 45.0f ) );
        mpImpl->mSub.SetPosition( Vector( -4.0f, 0.0f, 0.0f ) );
        mpImpl->mEntityList.push_back( &mpImpl->mSub );

        // Create axes to show coordinate system
        if ( !mpImpl->mAxes.Init( pSceneMgr ) )
        {
            fprintf( stderr, "Error: Unable to initialise coordinate system axes\n" );
            DeInit();
            return false;
        }
        mpImpl->mEntityList.push_back( &mpImpl->mAxes );
        
        if ( !mpImpl->mGate.Init( pSceneMgr ) )
        {
            fprintf( stderr, "Error: Unable to initialise gate\n" );
            DeInit();
            return false;
        }
        mpImpl->mGate.SetPosition( Vector( 0.0f, 15.0f, 0.0f ) );
        mpImpl->mEntityList.push_back( &mpImpl->mGate );
    
        if ( !mpImpl->mBuoy.Init( pSceneMgr ) )
        {
            fprintf( stderr, "Error: Unable to initialise buoy\n" );
            DeInit();
            return false;
        }
        mpImpl->mBuoy.SetPosition( Vector( 0.0f, 5.0f, 0.0f ) );
        mpImpl->mEntityList.push_back( &mpImpl->mBuoy );
    
        if ( !mpImpl->mPool.Init( pSceneMgr ) )
        {
            fprintf( stderr, "Error: Unable to initialise pool\n" );
            DeInit();
            return false;
        }
        mpImpl->mPool.SetPosition( Vector( 0.0f, -15.0f, 0.0f ) );
        mpImpl->mEntityList.push_back( &mpImpl->mPool );
    
        // Create some fog to represent underwater visibility
        pVideoDriver->setFog( irr::video::SColor( 0,0,25,220 ), 
                            irr::video::EFT_FOG_EXP, 50, 3000, 0.005f, true, false );
                            
        // Create camera to view scene
        mpImpl->mpCamera = pSceneMgr->addCameraSceneNodeMaya( 0, -750.0f, 200.0f, 300.0f ); 
        if ( NULL == mpImpl->mpCamera )
        {
            fprintf( stderr, "Error: Unable to initialise main camera\n" );
            DeInit();
            return false;
        }
        mpImpl->mpCamera->setTarget( irr::core::vector3df( 0, 0, 0 ) );
        
        mpImpl->mLastFPS = -1;
        mpImpl->mbIsRunning = true;
        
        mpImpl->mTimeAccumulatorUS = 0;
        mpImpl->mLastTime = HighPrecisionTime::GetTime();
        
        // Render to texture test
        mpImpl->mpTestCube = pSceneMgr->addCubeSceneNode( 20 );
        
        irr::scene::ISceneNodeAnimator* anim = 
            pSceneMgr->createRotationAnimator(irr::core::vector3df(0.3f, 0.3f,0));
        mpImpl->mpTestCube->setPosition(irr::core::vector3df(0,0,10));
        mpImpl->mpTestCube->setMaterialFlag(irr::video::EMF_LIGHTING, false); // disable dynamic lighting
        mpImpl->mpTestCube->addAnimator(anim);
        anim->drop();
        
        mpImpl->mpRenderTarget = NULL;
        mpImpl->mpFixedCam = NULL;
        
        if ( pVideoDriver->queryFeature( irr::video::EVDF_RENDER_TO_TARGET ) )
        {
            mpImpl->mpRenderTarget = pVideoDriver->addRenderTargetTexture(
                irr::core::dimension2d<U32>(256,256), "RTT1" );
            mpImpl->mpTestCube->setMaterialTexture(0, mpImpl->mpRenderTarget); // set material of cube to render target
                
            // add fixed camera
            mpImpl->mpFixedCam = pSceneMgr->addCameraSceneNode(
                0, irr::core::vector3df(10,10,-80), irr::core::vector3df(0,0,0));
        }
        else
        {
            fprintf( stderr, "Error: Render to texture not available... :(\n" );
        }
        
        mpImpl->mbInitialised = true;
    }
    
    return true;
}

//------------------------------------------------------------------------------
void Simulator::DeInit()
{
    if ( NULL != mpImpl->mpIrrDevice )
    {
        mpImpl->mpIrrDevice->drop(); 
        mpImpl->mpIrrDevice = NULL; 
    }
    
    mpImpl->mpText = NULL;
    
    mpImpl->mEntityList.clear();
    mpImpl->mSub.DeInit();
    mpImpl->mAxes.DeInit();
    mpImpl->mGate.DeInit();
    mpImpl->mBuoy.DeInit();
    mpImpl->mPool.DeInit();
    
    mpImpl->mpCamera = NULL;
    
    mpImpl->mbInitialised = false;
}

//------------------------------------------------------------------------------
void Simulator::Update()
{
    if ( mpImpl->mbInitialised
        && mpImpl->mbIsRunning )
    {
        if ( mpImpl->mpIrrDevice->run() )
        {
            S32 numUpdates = UpdateSimulator();
            UpdateFrameRender();
            UpdateFPSCounter( numUpdates );
        }
        else
        {
            mpImpl->mbIsRunning = false;
        }
    }
}

//--------------------------------------------------------------------------
S32 Simulator::UpdateSimulator()
{
    // Work out how many microseconds have elapsed since the last update
    HighPrecisionTime newTime = HighPrecisionTime::GetTime();
    HighPrecisionTime timeDiff = HighPrecisionTime::GetDiff( newTime,  mpImpl->mLastTime );
    S32 elapsedUS = HighPrecisionTime::ConvertToMicroSeconds( timeDiff );
    printf( "Time = %i, %i, Elapsed US = %i\n", newTime.mSeconds, newTime.mNanoSeconds, elapsedUS );
    
    // Add the elapsed time onto the accumulator and also cap the accumukator
    mpImpl->mTimeAccumulatorUS += elapsedUS;
    if ( mpImpl->mTimeAccumulatorUS > SIM_MAX_NUM_CATCHUP_FRAMES*SIM_MICRO_SECS_PER_SIM_FRAME )
    {
        mpImpl->mTimeAccumulatorUS = SIM_MAX_NUM_CATCHUP_FRAMES*SIM_MICRO_SECS_PER_SIM_FRAME;
    }
    
    S32 numUpdates = 0;
    
    // Simulate the required number of frames
    while ( mpImpl->mTimeAccumulatorUS >= SIM_MICRO_SECS_PER_SIM_FRAME )
    {
        // Update all of the entities in the simulator
        for ( EntityPtrVector::iterator entityIter = mpImpl->mEntityList.begin();
            mpImpl->mEntityList.end() != entityIter; ++entityIter )
        {
            (*entityIter)->Update( SIM_SECS_PER_SIM_FRAME );
        }
        
        mpImpl->mTimeAccumulatorUS -= SIM_MICRO_SECS_PER_SIM_FRAME;
        numUpdates++;
    }
    
    mpImpl->mLastTime = newTime;
    
    return numUpdates;
}

//--------------------------------------------------------------------------
void Simulator::UpdateFrameRender()
{
    irr::video::IVideoDriver* pVideoDriver = mpImpl->mpIrrDevice->getVideoDriver();
    irr::scene::ISceneManager* pSceneMgr = mpImpl->mpIrrDevice->getSceneManager();
    irr::gui::IGUIEnvironment* pGUIEnvironment = mpImpl->mpIrrDevice->getGUIEnvironment();

    const irr::video::SColor CLEAR_COLOUR(  255, 100, 101, 140 );
    pVideoDriver->beginScene( true, true, CLEAR_COLOUR );

    // Draw to render target
    if ( NULL != mpImpl->mpRenderTarget )
    {
        // draw scene into render target
                        
        // set render target texture
        pVideoDriver->setRenderTarget(mpImpl->mpRenderTarget, true, true, irr::video::SColor(0,0,0,255));
        
        // make cube invisible and set fixed camera as active camera
        mpImpl->mpTestCube->setVisible(false);
        pSceneMgr->setActiveCamera(mpImpl->mpFixedCam);
        
        // draw whole scene into render buffer
        pSceneMgr->drawAll();
        // set back old render target
        // The buffer might have been distorted, so clear it
        pVideoDriver->setRenderTarget(0, true, true, CLEAR_COLOUR);
        
        // make the cube visible and set the user controlled camera as active one
        mpImpl->mpTestCube->setVisible(true);
        pSceneMgr->setActiveCamera(mpImpl->mpCamera);
    }
    
    // Draw the rest of the scene normally
    pSceneMgr->drawAll();
    pGUIEnvironment->drawAll();
    
    pVideoDriver->endScene();
}
//--------------------------------------------------------------------------
void Simulator::UpdateFPSCounter( S32 numUpdates )
{
    static S32 updates = 0;
    
    updates += numUpdates;
    F32 numSecs = (F32)clock() / (F32)CLOCKS_PER_SEC;
    
    irr::video::IVideoDriver* pVideoDriver = mpImpl->mpIrrDevice->getVideoDriver();
    S32 fps = pVideoDriver->getFPS();
    if ( mpImpl->mLastFPS != fps )
    {
        irr::core::stringw str = L"Hello World - FPS: ";
        str += fps;
        str += " Num Updates: ";
        str += updates;
        str += " s ";
        str += numSecs;
        
        mpImpl->mpText->setText( str.c_str() );
        mpImpl->mLastFPS = fps;
    }
}

//------------------------------------------------------------------------------
bool Simulator::IsRunning() const
{
    bool bIsRunning = false;
    
    if ( mpImpl->mbInitialised )
    {
        bIsRunning = mpImpl->mbIsRunning;
    }
    
    return bIsRunning;
}

//--------------------------------------------------------------------------
void Simulator::SetSubForwardSpeed( F32 forwardSpeed )
{
    if ( mpImpl->mbInitialised )
    {
        mpImpl->mSub.SetForwardSpeed( forwardSpeed );
    }
}
    
//--------------------------------------------------------------------------
void Simulator::SetSubYawSpeed( F32 yawSpeed )
{
    if ( mpImpl->mbInitialised )
    {
        mpImpl->mSub.SetYawSpeed( yawSpeed );
    }
}