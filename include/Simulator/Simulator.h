//------------------------------------------------------------------------------
// File: Simulator.h
// Desc: The main simulator class
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef SIMULATOR_H
#define SIMULATOR_H

//------------------------------------------------------------------------------
#include "Common.h"

//------------------------------------------------------------------------------
struct SimulatorImpl;

//------------------------------------------------------------------------------
class Simulator
{
    //--------------------------------------------------------------------------
    public: Simulator();
    public: ~Simulator();

    //--------------------------------------------------------------------------
    public: bool Init( const char* worldFilename = NULL );
    public: void DeInit();

    //--------------------------------------------------------------------------
    public: void Update();
    
    //--------------------------------------------------------------------------
    // Helper routines
    private: S32 UpdateSimulator();
    private: void UpdateFrameRender();
    private: void UpdateFPSCounter( S32 numUpdates );
    
    //--------------------------------------------------------------------------
    // Returns true whilst the simulation is up and running
    public: bool IsRunning() const;
    
    //--------------------------------------------------------------------------
    // Interface for controlling the submarine
    //--------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------
    //! Sets the desired forward speed of the submarine in metres per second
    public: void SetSubForwardSpeed( F32 forwardSpeed );
    
    //--------------------------------------------------------------------------
    //! Sets the desired yaw speed of the submarine in radians per second
    public: void SetSubYawSpeed( F32 yawSpeed );
    
    //--------------------------------------------------------------------------
    //! Gets the time in seconds that the simulator has been running for.
    //! Can be used to timestamp data from interfaces
    public: double GetSimTime() const;
    
    // Returns (0,0) if no image is available
    public: void GetSubCameraImageDimensions( U32* pWidthOut, U32* pHeightOut ) const;
    
    public: void GetSubCameraImage( U8* pBufferInOut, U32 bufferSize ) const;

    //--------------------------------------------------------------------------
    // Members
    private: SimulatorImpl* mpImpl;
};

#endif // SIMULATOR_H