//------------------------------------------------------------------------------
// File: PresSensorInterface.h
// Desc: An interface that returns the depth of the simulated submarine
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef PRES_SENSOR_INTERFACE_H
#define PRES_SENSOR_INTERFACE_H

//------------------------------------------------------------------------------
#include "SubSimInterface.h"

//------------------------------------------------------------------------------
class PresSensorInterface : public SubSimInterface
{
    // Constructor
    public: PresSensorInterface( player_devaddr_t addr, SubSimDriver* pDriver,
                              ConfigFile* pConfigFile, int section );
    // Destructor
    public: virtual ~PresSensorInterface();

    // Handle all messages.
    public: virtual int ProcessMessage( QueuePointer &respQueue,
                                      player_msghdr_t* pHeader, void* pData );

    // Update this interface, publish new info.
    public: virtual void Update();
};

#endif // PRES_SENSOR_INTERFACE_H
