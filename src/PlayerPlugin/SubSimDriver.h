//------------------------------------------------------------------------------
// File: SubSimDriver.h
// Desc: A driver for Player so that it can use SubSim as a simulator
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef SUB_SIM_DRIVER_H
#define SUB_SIM_DRIVER_H

//------------------------------------------------------------------------------
#include <libplayercore/playercore.h>
#include "Simulator/Simulator.h"

//------------------------------------------------------------------------------
// Forward declarations
class SubSimInterface;

//------------------------------------------------------------------------------
class SubSimDriver : public Driver
{
    // Constructor/Destructor
    public: SubSimDriver( ConfigFile* pConfigFile, int section );
    public: virtual ~SubSimDriver();

    // Set up the device.  Return 0 if things go well, and -1 otherwise.
    public: virtual int Setup();

    // Shutdown the device
    public: virtual int Shutdown();

    // Process all messages for this driver. 
    public: virtual int ProcessMessage( QueuePointer& respQueue, 
                                        player_msghdr* pHeader, 
                                        void* pData );

    // The server thread calls this method frequently. We use it 
    // to check  for new commands and configs
    private: virtual void Update();

    // Helper function to load all devices on startup
    private: int LoadDevices( ConfigFile* pConfigFile, int section );

    // Find a device according to a player_devaddr
    private: SubSimInterface* LookupDevice( player_devaddr_t addr );

    // Array of interfaces associated with this driver
    protected: SubSimInterface** mpDeviceList;

    // Number of devices
    protected: int mNumDevices;

    // Max device count
    protected: int mMaxNumDevices;
    
    // Used to keep track of the rate at which we update the interfaces
    protected: double mLastInterfaceUpdateTime;
    
    public: Simulator mSim;
};

#endif // SUB_SIM_DRIVER_H
