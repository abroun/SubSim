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
    public: bool Init();
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
    // Members
    private: SimulatorImpl* mpImpl;
};

#endif // SIMULATOR_H