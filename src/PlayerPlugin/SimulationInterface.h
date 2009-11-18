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

    // Open a SHM interface when a subscription is received.
    // This is called from SubSimDriver::Subscribe
    public: virtual void Subscribe();

    // Close a SHM interface. This is called from SubSimDriver::Unsubscribe
    public: virtual void Unsubscribe();

    // Server Id. The name of the server
    private: char mServerIdString[ 1024 ];

    /// \brief Pointer to the Simulation Interface
    //public: SimulationIface *iface;

    //private: QueuePointer *responseQueue;

    private: player_simulation_pose3d_req_t mPose3dReq;

    private: player_simulation_pose2d_req_t mPose2dReq;
    //private: static boost::recursive_mutex *mutex;
};

#endif // SIMULATION_INTERFACE_H
