//------------------------------------------------------------------------------
// File: PresSensorInterface.cpp
// Desc: An interface that returns the depth of the simulated submarine
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "DepthSensorInterface.h"

#include <stdio.h>
#include "SubSimDriver.h"

//------------------------------------------------------------------------------
DepthSensorInterface::DepthSensorInterface( player_devaddr_t addr, 
    SubSimDriver* pDriver, ConfigFile* pConfigFile, int section )
    : SubSimInterface( addr, pDriver, pConfigFile, section )
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
    // Publish the current heading of the submarine
    player_position1d_data data;

    Vector subPos;
    Vector subRotation;
    mpDriver->mSim.GetEntityPose( "Sub", &subPos, &subRotation );
    
    data.pos = subPos.mZ;

    mpDriver->Publish( this->mDeviceAddress,
                       PLAYER_MSGTYPE_DATA, PLAYER_POSITION1D_DATA_STATE,
                       (void*)&data, sizeof( data ) );
}



