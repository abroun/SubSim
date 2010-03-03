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
#include "Entities/FloorTarget.h"
#include "CameraSceneNodeAnimator.h"

#include <btBulletDynamicsCommon.h>

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
    Gate mGate1;
    Gate mGate2;
    Buoy mBuoy;
    Pool mPool;
    FloorTarget mFloorTarget;
    EntityPtrVector mEntityList;
    
    // TODO: Tidy up the timing.
    HighPrecisionTime mSimulatorStartTime;
    HighPrecisionTime mLastTime;
    S32 mTimeAccumulatorUS; // The number of microseconds that we need to deal with in the next update
    
    S32 mLastFPS;
    bool mbIsRunning;
    
    // Physics stuff
    btDefaultCollisionConfiguration* mpCollisionConf;
    btCollisionDispatcher* mpCollisionDispatcher;
    btBroadphaseInterface* mpOverlappingPairCache;
    btSequentialImpulseConstraintSolver* mpPhysicsSolver;
    btDiscreteDynamicsWorld* mpPhysicsWorld;  
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
    
    mpImpl->mpCollisionConf = NULL;
    mpImpl->mpCollisionDispatcher = NULL;
    mpImpl->mpOverlappingPairCache = NULL;
    mpImpl->mpPhysicsSolver = NULL;
    mpImpl->mpPhysicsWorld = NULL;
    
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
bool Simulator::Init( const char* worldFilename )
{
    // Open up world file
    // Sequence Start
    // Identify element type
    // Read in entity attributes
    // Create entity
    
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
    
        // Setup physics for the world
        mpImpl->mpCollisionConf = new btDefaultCollisionConfiguration();
        mpImpl->mpCollisionDispatcher = new btCollisionDispatcher( mpImpl->mpCollisionConf );
        mpImpl->mpOverlappingPairCache = new btDbvtBroadphase();
        mpImpl->mpPhysicsSolver = new btSequentialImpulseConstraintSolver();
        mpImpl->mpPhysicsWorld = new btDiscreteDynamicsWorld(
            mpImpl->mpCollisionDispatcher, mpImpl->mpOverlappingPairCache,
            mpImpl->mpPhysicsSolver, mpImpl->mpCollisionConf );
        
        mpImpl->mpPhysicsWorld->setGravity( btVector3( 0.0f, 0.0f, 0.0f ) );
            
        // Setup sub
        if ( !mpImpl->mSub.Init( pSceneMgr, pVideoDriver ) )
        {
            fprintf( stderr, "Error: Unable to initialise sub\n" );
            DeInit();
            return false;
        }
        mpImpl->mSub.SetYaw( MathUtils::DegToRad( 45.0f ) );
        mpImpl->mSub.SetPosition( Vector( 0.0f, 0.0f, -1.0f ) );
        mpImpl->mEntityList.push_back( &mpImpl->mSub );

        // Create axes to show coordinate system
        if ( !mpImpl->mAxes.Init( pSceneMgr ) )
        {
            fprintf( stderr, "Error: Unable to initialise coordinate system axes\n" );
            DeInit();
            return false;
        }
        mpImpl->mAxes.SetPosition( Vector( -10.0f, 0.0f, 0.0f ) );
        mpImpl->mEntityList.push_back( &mpImpl->mAxes );
        
        if ( !mpImpl->mGate.Init( pSceneMgr ) )
        {
            fprintf( stderr, "Error: Unable to initialise gate\n" );
            DeInit();
            return false;
        }
        mpImpl->mGate.SetPosition( Vector( 0.0f, 4.0f, -2.25f ) );
        
        if ( !mpImpl->mGate1.Init( pSceneMgr ) )
        {
            fprintf( stderr, "Error: Unable to initialise gate\n" );
            DeInit();
            return false;
        }
        mpImpl->mGate1.SetPosition( Vector( 0.0f, 14.0f, -2.25f ) );
        
        if ( !mpImpl->mGate2.Init( pSceneMgr ) )
        {
            fprintf( stderr, "Error: Unable to initialise gate\n" );
            DeInit();
            return false;
        }
        mpImpl->mGate2.SetPosition( Vector( 0.0f, 24.0f, -2.25f ) );
        
        mpImpl->mEntityList.push_back( &mpImpl->mGate );
        mpImpl->mEntityList.push_back( &mpImpl->mGate1 );
        mpImpl->mEntityList.push_back( &mpImpl->mGate2 );
        
        if ( !mpImpl->mFloorTarget.Init( pSceneMgr ) )
        {
            fprintf( stderr, "Error: Unable to initialise floor target\n" );
            DeInit();
            return false;
        }
        mpImpl->mFloorTarget.SetPosition( Vector( 0.0f, 0.0f, -5.8f ) );
    
        if ( !mpImpl->mBuoy.Init( pSceneMgr, mpImpl->mpPhysicsWorld ) )
        {
            fprintf( stderr, "Error: Unable to initialise buoy\n" );
            DeInit();
            return false;
        }
        mpImpl->mBuoy.SetPosition( Vector( 10.0f, -5.0f, -1.0f ) );
        mpImpl->mEntityList.push_back( &mpImpl->mBuoy );
    
        if ( !mpImpl->mPool.Init( pSceneMgr ) )
        {
            fprintf( stderr, "Error: Unable to initialise pool\n" );
            DeInit();
            return false;
        }
        mpImpl->mPool.SetPosition( Vector( 0.0f, 15.0f, -2.5f ) );
        mpImpl->mEntityList.push_back( &mpImpl->mPool );
    
        // Create some fog to represent underwater visibility
        pVideoDriver->setFog( irr::video::SColor( 0,0,25,220 ), 
                            irr::video::EFT_FOG_EXP, 50, 3000, 0.005f, true, false );
                 
                            //  -750.0f, 200.0f, 300.0f ); 
                            
        // Create camera to view scene
        mpImpl->mpCamera = pSceneMgr->addCameraSceneNode( 0,
            irr::core::vector3df( 10.0f, 0, 0 ), irr::core::vector3df( 0, 0, 0 ) );
        if ( NULL == mpImpl->mpCamera )
        {
            fprintf( stderr, "Error: Unable to initialise main camera\n" );
            DeInit();
            return false;
        }
        
        irr::scene::ISceneNodeAnimator* anm = new irr::scene::CameraSceneNodeAnimator(
            mpImpl->mpIrrDevice->getCursorControl(), 
            irr::core::vector3df( 10.0f, 0, 0 ), -750.0f, 200.0f, 300.0f );

        mpImpl->mpCamera->addAnimator(anm);
        
        mpImpl->mLastFPS = -1;
        mpImpl->mbIsRunning = true;
        
        mpImpl->mTimeAccumulatorUS = 0;
        mpImpl->mSimulatorStartTime = HighPrecisionTime::GetTime();
        mpImpl->mLastTime = mpImpl->mSimulatorStartTime;      
        
        mpImpl->mbInitialised = true;
    }
    
    return true;
}

//------------------------------------------------------------------------------
void Simulator::DeInit()
{
    mpImpl->mEntityList.clear();
    mpImpl->mSub.DeInit();
    mpImpl->mAxes.DeInit();
    mpImpl->mGate.DeInit();
    mpImpl->mBuoy.DeInit();
    mpImpl->mPool.DeInit();
    
    mpImpl->mpCamera = NULL;
    
    if ( NULL == mpImpl->mpPhysicsWorld )
    {
        delete mpImpl->mpPhysicsWorld;
        mpImpl->mpPhysicsWorld = NULL;
    }
    
    if ( NULL == mpImpl->mpPhysicsSolver )
    {
        delete mpImpl->mpPhysicsSolver;
        mpImpl->mpPhysicsSolver = NULL;
    }
    
    if ( NULL == mpImpl->mpOverlappingPairCache )
    {
        delete mpImpl->mpOverlappingPairCache;
        mpImpl->mpOverlappingPairCache = NULL;
    }
    
    if ( NULL == mpImpl->mpCollisionDispatcher )
    {
        delete mpImpl->mpCollisionDispatcher;
        mpImpl->mpCollisionDispatcher = NULL;
    }
    
    if ( NULL == mpImpl->mpCollisionConf )
    {
        delete mpImpl->mpCollisionConf;
        mpImpl->mpCollisionConf = NULL;
    }
    
    mpImpl->mpText = NULL;
    
    if ( NULL != mpImpl->mpIrrDevice )
    {
        mpImpl->mpIrrDevice->drop(); 
        mpImpl->mpIrrDevice = NULL; 
    }
    
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
    //printf( "Time = %i, %i, Elapsed US = %i\n", newTime.mSeconds, newTime.mNanoSeconds, elapsedUS );
    
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

    // Render the view from the submarine's camera
    irr::video::ITexture* pSubCameraRenderTarget = mpImpl->mSub.GetCameraRenderTarget();
    if ( NULL != pSubCameraRenderTarget )
    {                        
        // Set render target texture
        pVideoDriver->setRenderTarget( pSubCameraRenderTarget, 
                                       true, true, CLEAR_COLOUR );
        
        // Set sub camera as active camera
        pSceneMgr->setActiveCamera( mpImpl->mSub.GetCameraNode() );
        
        // Draw whole scene into render buffer
        pSceneMgr->drawAll();
        // Set back old render target
        // The buffer might have been distorted, so clear it
        pVideoDriver->setRenderTarget( 0, true, true, CLEAR_COLOUR );
        pSceneMgr->setActiveCamera( mpImpl->mpCamera );
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
bool BuildWorld( const char* worldFilename )
{
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

//--------------------------------------------------------------------------
double Simulator::GetSimTime() const
{
    return HighPrecisionTime::ConvertToSeconds( 
        HighPrecisionTime::GetDiff( 
            mpImpl->mLastTime, mpImpl->mSimulatorStartTime ) );
}

//--------------------------------------------------------------------------
void Simulator::GetSubCameraImageDimensions( U32* pWidthOut, U32* pHeightOut ) const
{
    *pWidthOut = 0;
    *pHeightOut = 0;
    
    if ( mpImpl->mbInitialised )
    {
        irr::video::ITexture* pSubCameraRenderTarget = mpImpl->mSub.GetCameraRenderTarget();
        if ( NULL != pSubCameraRenderTarget )
        {                        
            irr::core::dimension2d<U32> imageSize =
                pSubCameraRenderTarget->getSize();
            
            *pWidthOut = imageSize.Width;
            *pHeightOut = imageSize.Height;
        }
    }
}
    
//--------------------------------------------------------------------------
void Simulator::GetSubCameraImage( U8* pBufferInOut, U32 bufferSize ) const
{
    if ( mpImpl->mbInitialised )
    {
        irr::video::ITexture* pSubCameraRenderTarget = mpImpl->mSub.GetCameraRenderTarget();
        if ( NULL != pSubCameraRenderTarget )
        {
            irr::core::dimension2d<U32> imageSize =
                pSubCameraRenderTarget->getSize();
                
            U32 requiredBufferSize = imageSize.Width*imageSize.Height*3;
            if ( bufferSize < requiredBufferSize )
            {
                fprintf( stderr, "Warning: Supplied buffer is too small\n ");
                return;
            }
            
            U8* pImageData = (U8*)pSubCameraRenderTarget->lock( true );
            if ( NULL == pImageData )
            {
                fprintf( stderr, "Warning: Unable to lock camera texture for reading\n ");
                return;
            }
            
            U32 imagePitch = pSubCameraRenderTarget->getPitch();
            switch ( pSubCameraRenderTarget->getColorFormat() )
            {
                case irr::video::ECF_A1R5G5B5:
                {
                    for ( U32 rowIdx = 0; rowIdx < imageSize.Height; rowIdx++ )
                    {
                        U16* pSrcPixel = (U16*)(pImageData + imagePitch*rowIdx);
                        U8* pDstPixel = pBufferInOut + 3*imageSize.Width*rowIdx;
                        U8* pLastPixel = pDstPixel + 3*imageSize.Width;
                        
                        while ( pDstPixel < pLastPixel )
                        {
                            *pDstPixel++ = (U8)( ( *pSrcPixel & 0x00007C00 ) >> 10 ); // R
                            *pDstPixel++ = (U8)( ( *pSrcPixel & 0x000003E0 ) >> 5 );  // G
                            *pDstPixel++ = (U8)( ( *pSrcPixel & 0x0000001F ) );       // B
                            pSrcPixel++;
                        }
                    }
                    
                    break;
                }
                case irr::video::ECF_A8R8G8B8:
                {
                    for ( U32 rowIdx = 0; rowIdx < imageSize.Height; rowIdx++ )
                    {
                        U32* pSrcPixel = (U32*)(pImageData + imagePitch*rowIdx);
                        U8* pDstPixel = pBufferInOut + 3*imageSize.Width*rowIdx;
                        U8* pLastPixel = pDstPixel + 3*imageSize.Width;
                        
                        while ( pDstPixel < pLastPixel )
                        {
                            irr::video::SColor sourceColour( *pSrcPixel );
                            
                            //U8* pSrcPixelData = (U8*)pSrcPixel;
                            *pDstPixel++ = sourceColour.getRed(); // pSrcPixelData[ 1 ]; // R
                            *pDstPixel++ = sourceColour.getGreen(); // pSrcPixelData[ 2 ]; // G
                            *pDstPixel++ = sourceColour.getBlue(); // pSrcPixelData[ 3 ]; // B
                            pSrcPixel++;
                        }
                    }
                    
                    break;
                }
                default:
                {
                    fprintf( stderr, "Warning: Unhandled colour format for camera texture, 0x%X\n",
                             pSubCameraRenderTarget->getColorFormat() );
                }
            }
            
            pSubCameraRenderTarget->unlock();
        }
    }
}