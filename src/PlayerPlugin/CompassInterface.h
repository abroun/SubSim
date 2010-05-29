//------------------------------------------------------------------------------
// File: CompassInterface.h
// Desc: An interface that returns the heading of the simulated submarine
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef COMPASS_INTERFACE_H
#define COMPASS_INTERFACE_H

//------------------------------------------------------------------------------
#include "SubSimInterface.h"

//------------------------------------------------------------------------------
class CompassInterface : public SubSimInterface
{
    // Constructor
    public: CompassInterface( player_devaddr_t addr, SubSimDriver* pDriver,
                              ConfigFile* pConfigFile, int section );
    // Destructor
    public: virtual ~CompassInterface();

    // Handle all messages.
    public: virtual int ProcessMessage( QueuePointer &respQueue,
                                      player_msghdr_t* pHeader, void* pData );

    // Update this interface, publish new info.
    public: virtual void Update();
};

#endif // COMPASS_INTERFACE_H
