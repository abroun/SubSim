//------------------------------------------------------------------------------
// File: SubSimInterface.h
// Desc: Generic SubSim device inteface for Player
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef SUB_SIM_INTERFACE_H
#define SUB_SIM_INTERFACE_H

//------------------------------------------------------------------------------
#include <libplayercore/playercore.h>

//------------------------------------------------------------------------------
// Forward declarations
class SubSimDriver;

//------------------------------------------------------------------------------
// Base class for all the Player interfaces
class SubSimInterface
{
    // Constructor
    public: SubSimInterface( player_devaddr_t addr, SubSimDriver* pDriver, 
        ConfigFile* pConfigFile, int section );

    // Destructor
    public: virtual ~SubSimInterface();

    //  Handle all messages.
    public: virtual int ProcessMessage( QueuePointer &respQueue,
                                     player_msghdr_t* pHeader, void* pData ) = 0;

    public: virtual void Subscribe() {};
    public: virtual void Unsubscribe() {};
    public: virtual void Subscribe( QueuePointer& respQueue ) {}; 
    public: virtual void Unsubscribe( QueuePointer& respQueue ) {};
                                     
    // Update this interface, publish new info.
    public: virtual void Update() = 0;

    // Address of the Player Device
    public: player_devaddr_t mDeviceAddress;

    // Driver instance that created this device
    public: SubSimDriver* mpDriver;
};

#endif // SUB_SIM_INTERFACE_H
