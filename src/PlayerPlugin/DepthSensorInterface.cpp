//------------------------------------------------------------------------------
// File: PresSensorInterface.cpp
// Desc: An interface that returns the depth of the simulated submarine
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "DepthSensorInterface.h"

#include <stdio.h>
#include "SubSimDriver.h"

//------------------------------------------------------------------------------
const F32 DepthSensorInterface::TIME_UNTILL_DEPTH_SENSOR_BREAKS = -15.0f;

//------------------------------------------------------------------------------
DepthSensorInterface::DepthSensorInterface( player_devaddr_t addr, 
    SubSimDriver* pDriver, ConfigFile* pConfigFile, int section )
    : SubSimInterface( addr, pDriver, pConfigFile, section ),
    mbDepthSensorBroken( false ),
    mLastValue( 0.0f ),
    mWaitForBreakStartTime( HighPrecisionTime::GetTime() )
{
}

//------------------------------------------------------------------------------
DepthSensorInterface::~DepthSensorInterface()
{
}

//------------------------------------------------------------------------------
// Handle all messages.
int DepthSensorInterface::ProcessMessage( QueuePointer& respQueue,
                                        player_msghdr_t* pHeader, void* pData )
{
    
    printf( "Unhandled message\n" );
    return -1;
}


//------------------------------------------------------------------------------
// Update this interface and publish new info.
void DepthSensorInterface::Update()
{
    if ( TIME_UNTILL_DEPTH_SENSOR_BREAKS > 0.0f )
    {
        HighPrecisionTime curTime = HighPrecisionTime::GetTime();
        F32 timeDiff = HighPrecisionTime::ConvertToSeconds(
            HighPrecisionTime::GetDiff( curTime, mWaitForBreakStartTime ) );
        if ( timeDiff > TIME_UNTILL_DEPTH_SENSOR_BREAKS )
        {
            mbDepthSensorBroken = true;
        }
    }
        
    // Publish the current heading of the submarine
    player_position1d_data data;

    Vector subPos;
    Vector subRotation;
    mpDriver->mSim.GetEntityPose( "Sub", &subPos, &subRotation );
    
    // Convert to positive depth to make the depth sensor more like the 
    // real one
    if ( mbDepthSensorBroken )
    {
        data.pos = mLastValue;
    }
    else
    {
        data.pos = -subPos.mZ*1000;
        
        // Add in some noise so that the control code doesn't think the
        // depth sensor has crashed
        F32 noise = 1.0f*(F32)random()/(F32)RAND_MAX - 0.5f;
        data.pos += noise;
    }
    mLastValue = data.pos;

    mpDriver->Publish( this->mDeviceAddress,
                       PLAYER_MSGTYPE_DATA, PLAYER_POSITION1D_DATA_STATE,
                       (void*)&data, sizeof( data ) );
}



