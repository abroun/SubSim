//------------------------------------------------------------------------------
// File: SimulationInterface.h
// Desc: An interface that allows Player to poke under the covers and move
//       simulation entities arround
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef SIMULATION_INTERFACE_H
#define SIMULATION_INTERFACE_H

//------------------------------------------------------------------------------
#include "SubSimInterface.h"

/// \addtogroup player_iface 
/// \{
/// \defgroup simulation_player Simulation Interface
/// Provides:
///  - PLAYER_SIMULATION_REQ_SET_POSE3D
///    - "set_pose3d"
///  - PLAYER_SIMULATION_REQ_SET_POSE2D
///    - "set_pose2d"
///  - PLAYER_SIMULATION_REQ_GET_POSE3D
///    - "get_pose"
///  - PLAYER_SIMULATION_REQ_GET_POSE2D
///    - "get_pose"
///  - PLAYER_SIMULATION_REQ_GET_PROPERTY
///    - "sim_time" returns double
///    - "real_time" returns double
///    - "pause_time" returns double

//------------------------------------------------------------------------------
class SubSimDriver;

//------------------------------------------------------------------------------
class SimulationInterface : public SubSimInterface
{
    // Constructor
    public: SimulationInterface( player_devaddr_t addr, SubSimDriver* pDriver,
                              ConfigFile* pConfigFile, int section );
    // Destructor
    public: virtual ~SimulationInterface();

    // Handle all messages. This is called from SubSimDriver
    public: virtual int ProcessMessage( QueuePointer &respQueue,
                                      player_msghdr_t* pHeader, void* pData );

    // Update this interface, publish new info.
    public: virtual void Update();
};

#endif // SIMULATION_INTERFACE_H
