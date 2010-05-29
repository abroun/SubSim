//------------------------------------------------------------------------------
// File: CompassInterface.cpp
// Desc: An interface that returns the heading of the simulated submarine
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "CompassInterface.h"

#include <stdio.h>
#include "SubSimDriver.h"

//------------------------------------------------------------------------------
CompassInterface::CompassInterface( player_devaddr_t addr, 
    SubSimDriver* pDriver, ConfigFile* pConfigFile, int section )
    : SubSimInterface( addr, pDriver, pConfigFile, section )
{
}

//------------------------------------------------------------------------------
CompassInterface::~CompassInterface()
{
}

//------------------------------------------------------------------------------
// Handle all messages.
int CompassInterface::ProcessMessage( QueuePointer& respQueue,
                                        player_msghdr_t* pHeader, void* pData )
{
    
    printf( "Unhandled message\n" );
    return -1;
}


//------------------------------------------------------------------------------
// Update this interface and publish new info.
void CompassInterface::Update()
{
    // Publish the current heading of the submarine
    player_imu_data_state_t data;

    Vector subPos;
    Vector subRotation;
    mpDriver->mSim.GetEntityPose( "Sub", &subPos, &subRotation );
    
    data.pose.px = 0.0;
    data.pose.py = 0.0;
    data.pose.pz = 0.0;

    data.pose.proll = 0.0;
    data.pose.ppitch = 0.0;
    data.pose.pyaw = subRotation.mZ;

    mpDriver->Publish( this->mDeviceAddress,
                       PLAYER_MSGTYPE_DATA, PLAYER_IMU_DATA_STATE,
                       (void*)&data, sizeof( data ) );
}



