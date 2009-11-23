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
#include "Entities/Sub.h"
#include "Entities/CoordinateSystemAxes.h"
#include "Entities/Gate.h"
#include "Entities/Buoy.h"
#include "Entities/Pool.h"

//------------------------------------------------------------------------------
// Constants and Typdefs
//------------------------------------------------------------------------------
static F32 SIM_DESIRED_SIM_FPS = 30.0f;
static S32 SIM_MS_PER_SIM_FRAME = (S32)(1000.0f / SIM_DESIRED_SIM_FPS);
static S32 SIM_MAX_NUM_CATCHUP_FRAMES = 30; // If the simulator gets more than
                                            // this number of frames behind it
                                            // will start dropping frames

typedef std::vector<Entity*> EntityPtrVector;

//------------------------------------------------------------------------------
// Helper Routines
//------------------------------------------------------------------------------
// Converts a clock_t time to milliseconds
static S32 SIM_ClockTicksToMS( clock_t numTicks )
{
    const F32 CLK_PER_MS = (F32)CLOCKS_PER_SEC / 1000.0f;
    
    return (S32)((F32)numTicks / CLK_PER_MS);
}

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
    
    clock_t mLastClock; // The clock time
    S32 mTimeAccumulatorMS; // The number of milliseconds that we need to deal with in the next update
    
    S32 mLastFPS;
    bool mbIsRunning;
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
        
        mpImpl->mTimeAccumulatorMS = 0;
        mpImpl->mLastClock = clock();
        
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
    // Work out how many milliseconds have elapsed since the last update
    clock_t newClock = clock();
    clock_t clockDiff = newClock - mpImpl->mLastClock;
    S32 elapsedMS = SIM_ClockTicksToMS( newClock - mpImpl->mLastClock );
    //printf( "Clock = %i Elapsed MS = %i\n", (S32)clockDiff, elapsedMS );
    
    elapsedMS = SIM_MS_PER_SIM_FRAME;
    
    // Add the elapsed time onto the accumulator and also cap the accumukator
    mpImpl->mTimeAccumulatorMS += elapsedMS;
    if ( mpImpl->mTimeAccumulatorMS > SIM_MAX_NUM_CATCHUP_FRAMES*SIM_MS_PER_SIM_FRAME )
    {
        mpImpl->mTimeAccumulatorMS = SIM_MAX_NUM_CATCHUP_FRAMES*SIM_MS_PER_SIM_FRAME;
    }
    
    S32 numUpdates = 0;
    
    // Simulate the required number of frames
    while ( mpImpl->mTimeAccumulatorMS >= SIM_MS_PER_SIM_FRAME )
    {
        // Update all of the entities in the simulator
        for ( EntityPtrVector::iterator entityIter = mpImpl->mEntityList.begin();
            mpImpl->mEntityList.end() != entityIter; ++entityIter )
        {
            (*entityIter)->Update( SIM_MS_PER_SIM_FRAME );
        }
        
        mpImpl->mTimeAccumulatorMS -= SIM_MS_PER_SIM_FRAME;
        numUpdates++;
    }
    
    mpImpl->mLastClock = newClock;
    
    return numUpdates;
}

//--------------------------------------------------------------------------
void Simulator::UpdateFrameRender()
{
    irr::video::IVideoDriver* pVideoDriver = mpImpl->mpIrrDevice->getVideoDriver();
    irr::scene::ISceneManager* pSceneMgr = mpImpl->mpIrrDevice->getSceneManager();
    irr::gui::IGUIEnvironment* pGUIEnvironment = mpImpl->mpIrrDevice->getGUIEnvironment();

    pVideoDriver->beginScene( true, true, irr::video::SColor( 255, 100, 101, 140 ) );

    pSceneMgr->drawAll();
    pGUIEnvironment->drawAll();
    
    pVideoDriver->endScene();
}
//--------------------------------------------------------------------------
void Simulator::UpdateFPSCounter( S32 numUpdates )
{
    irr::video::IVideoDriver* pVideoDriver = mpImpl->mpIrrDevice->getVideoDriver();
    S32 fps = pVideoDriver->getFPS();
    if ( mpImpl->mLastFPS != fps )
    {
        irr::core::stringw str = L"Hello World - FPS: ";
        str += fps;
        str += " Num Updates: ";
        str += numUpdates;
        
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