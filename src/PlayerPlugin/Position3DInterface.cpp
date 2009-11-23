//------------------------------------------------------------------------------
// File: Position3DInterface.cpp
// Desc: An interface that allows Player to move a simulation entity around
//       in 3D
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "Position3DInterface.h"

#include <stdio.h>
#include "SubSimDriver.h"

//------------------------------------------------------------------------------
Position3DInterface::Position3DInterface( player_devaddr_t addr, 
    SubSimDriver* pDriver, ConfigFile* pConfigFile, int section )
    : SubSimInterface( addr, pDriver, pConfigFile, section )
{
}

//------------------------------------------------------------------------------
Position3DInterface::~Position3DInterface()
{
}

//------------------------------------------------------------------------------
// Handle all messages.
int Position3DInterface::ProcessMessage( QueuePointer& respQueue,
                                        player_msghdr_t* pHeader, void* pData )
{
    // New motor command - velocity
    if( Message::MatchMessage( pHeader, PLAYER_MSGTYPE_CMD, 
                           PLAYER_POSITION3D_CMD_SET_VEL, 
                           mDeviceAddress ) )
    {
        player_position3d_cmd_vel_t* pCmd = (player_position3d_cmd_vel_t*)pData;
        
        printf( "Set vel = %2.3f, %2.3f, %2.3f\n",
            (F32)pCmd->vel.px, (F32)pCmd->vel.py, (F32)pCmd->vel.pz );
        
        mpDriver->mSim.SetSubForwardSpeed( (F32)pCmd->vel.pz );
        mpDriver->mSim.SetSubYawSpeed( (F32)pCmd->vel.pyaw );

        return 0;
    }
    // Request to enable motor power
    else if( Message::MatchMessage( pHeader, PLAYER_MSGTYPE_REQ, 
                                PLAYER_POSITION3D_REQ_MOTOR_POWER, 
                                mDeviceAddress ) )
    {
        printf( "Ignoring motor request" );
        mpDriver->Publish( mDeviceAddress, respQueue, 
                         PLAYER_MSGTYPE_RESP_ACK, 
                         PLAYER_POSITION3D_REQ_MOTOR_POWER );
        return 0;
    }

  /*// Is it a new motor command?
  else if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD, 
                           PLAYER_POSITION2D_CMD_CAR, 
                           this->addr))
  {
    // convert from Player to Stage format
    player_position2d_cmd_car_t* pcmd = (player_position2d_cmd_car_t*)data;

    mod->SetSpeed( pcmd->velocity, 0, pcmd->angle );
    return 0;
  }
 
  // Is it a request for position geometry?
  else if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, 
                                PLAYER_POSITION2D_REQ_GET_GEOM, 
                                this->addr))
  {
    if(hdr->size == 0)
    {
      Geom geom = this->mod->GetGeom();

      // fill in the geometry data formatted player-like
      player_position2d_geom_t pgeom;
      pgeom.pose.px = geom.pose.x;
      pgeom.pose.py = geom.pose.y;
      pgeom.pose.pyaw = geom.pose.a;

      pgeom.size.sl = geom.size.x; 
      pgeom.size.sw = geom.size.y; 

      this->driver->Publish( this->addr, resp_queue,
                             PLAYER_MSGTYPE_RESP_ACK, 
                             PLAYER_POSITION2D_REQ_GET_GEOM,
                             (void*)&pgeom, sizeof(pgeom), NULL );
      return 0;
    }
    else
    {
      PRINT_ERR2("config request len is invalid (%d != %d)", 
         (int)hdr->size, 0);
      return(-1);
    }
  }
  // Is it a request to reset odometry?
  else if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, 
                                PLAYER_POSITION2D_REQ_RESET_ODOM, 
                                this->addr))
  {
    if(hdr->size == 0)
    {
      PRINT_DEBUG( "resetting odometry" );

      mod->est_pose.x = 0;
      mod->est_pose.y = 0;
      mod->est_pose.z = 0;
      mod->est_pose.a = 0;

      this->driver->Publish( this->addr, resp_queue, 
                             PLAYER_MSGTYPE_RESP_ACK,
                             PLAYER_POSITION2D_REQ_RESET_ODOM );
      return 0;
    }
    else
    {
      PRINT_ERR2("config request len is invalid (%d != %d)", 
                 (int)hdr->size, 0);
      return -1;
    }
  }
  // Is it a request to set odometry?
  else if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, 
                                PLAYER_POSITION2D_REQ_SET_ODOM, 
                                this->addr))
  {
    if(hdr->size == sizeof(player_position2d_set_odom_req_t))
    {
      player_position2d_set_odom_req_t* req = 
              (player_position2d_set_odom_req_t*)data;

        
      mod->est_pose.x = req->pose.px;
      mod->est_pose.y = req->pose.py;
      //mod->est_pose.z = req->pose.pz;
      mod->est_pose.a = req->pose.pa;

      PRINT_DEBUG3( "set odometry to (%.2f,%.2f,%.2f)",
                    pose.x,
                    pose.y,
                    pose.a );

      this->driver->Publish( this->addr, resp_queue, 
                             PLAYER_MSGTYPE_RESP_ACK, 
                             PLAYER_POSITION2D_REQ_SET_ODOM );
      return(0);
    }
    else
    {
      PRINT_ERR2("config request len is invalid (%d != %d)", 
                 (int)hdr->size, (int)sizeof(player_position2d_set_odom_req_t));
      return(-1);
    }
  }
  // Is it a request to enable motor power?
  else if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, 
                                PLAYER_POSITION2D_REQ_MOTOR_POWER, 
                                this->addr))
  {
    if(hdr->size == sizeof(player_position2d_power_config_t))
    {
      player_position2d_power_config_t* req = 
              (player_position2d_power_config_t*)data;

      int motors_on = req->state;

      PRINT_WARN1( "Stage ignores motor power state (%d)",
                   motors_on );
      this->driver->Publish( this->addr, resp_queue, 
                             PLAYER_MSGTYPE_RESP_ACK, 
                             PLAYER_POSITION2D_REQ_MOTOR_POWER );
      return(0);
    }
    else
    {
      PRINT_ERR2("config request len is invalid (%d != %d)", 
                 (int)hdr->size, (int)sizeof(player_position2d_power_config_t));
      return(-1);
    }
  }
  // Is it a request to switch control mode?
  else if(Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ, 
                                PLAYER_POSITION2D_REQ_POSITION_MODE, 
                                this->addr))
  {
    if(hdr->size == sizeof(player_position2d_position_mode_req_t))
    {

      //player_position2d_position_mode_req_t* req = 
        //      (player_position2d_position_mode_req_t*)data;

      //stg_position_control_mode_t mode = (stg_position_control_mode_t)req->state;


      PRINT_WARN2( "Put model %s into %s control mode", this->mod->Token(), mod ? "POSITION" : "VELOCITY" );
      PRINT_WARN( "set control mode not yet implemented") ;

      this->driver->Publish( this->addr, resp_queue, 
                             PLAYER_MSGTYPE_RESP_ACK, 
                             PLAYER_POSITION2D_REQ_POSITION_MODE );
      return(0);
    }
    else
    {
      PRINT_ERR2("config request len is invalid (%d != %d)", 
                 (int)hdr->size, 
                 (int)sizeof(player_position2d_position_mode_req_t));
      return(-1);
    }
  }
  
  //else
  
  // Don't know how to handle this message.
  PRINT_WARN2( "stg_position doesn't support msg with type %d subtype %d",
           hdr->type, hdr->subtype);
  return(-1);*/
    
    printf( "Unhandled message\n" );
    return -1;
}


//------------------------------------------------------------------------------
// Update this interface and publish new info.
void Position3DInterface::Update()
{
    // We don't actually publish anything useful here. After all, we don't know
    // yet if the Sub will do anything other than just take desired velocity
    // commands. However, some clients will block till they get fresh data...
    player_position3d_data_t data;

    data.pos.px = 0.0;
    data.pos.py = 0.0;
    data.pos.pz = 0.0;

    data.pos.proll = 0.0;
    data.pos.ppitch = 0.0;
    data.pos.pyaw = 0.0;

    data.vel.px = 0.0;
    data.vel.py = 0.0;
    data.vel.pz = 0.0;

    data.vel.proll = 0.0;
    data.vel.ppitch = 0.0;
    data.vel.pyaw = 0.0;

    data.stall = (uint8_t)0;

    mpDriver->Publish( this->mDeviceAddress,
                       PLAYER_MSGTYPE_DATA, PLAYER_POSITION3D_DATA_STATE,
                       (void*)&data, sizeof( data ) );
}



