//------------------------------------------------------------------------------
// File: DepthSensorInterface.h
// Desc: An interface that returns the depth of the simulated submarine
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef DEPTH_SENSOR_INTERFACE_H
#define DEPTH_SENSOR_INTERFACE_H

//------------------------------------------------------------------------------
#include "SubSimInterface.h"

//------------------------------------------------------------------------------
class DepthSensorInterface : public SubSimInterface
{
    // Constructor
    public: DepthSensorInterface( player_devaddr_t addr, SubSimDriver* pDriver,
                              ConfigFile* pConfigFile, int section );
    // Destructor
    public: virtual ~DepthSensorInterface();

    // Handle all messages.
    public: virtual int ProcessMessage( QueuePointer &respQueue,
                                      player_msghdr_t* pHeader, void* pData );

    // Update this interface, publish new info.
    public: virtual void Update();
};

#endif // DEPTH_SENSOR_INTERFACE_H
