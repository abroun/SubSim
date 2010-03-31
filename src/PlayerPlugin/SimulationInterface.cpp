//------------------------------------------------------------------------------
// File: SimulationInterface.cpp
// Desc: An interface that allows Player to poke under the covers and move
//       simulation entities arround
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include <time.h>
//#include <iostream>
//#include <boost/thread/recursive_mutex.hpp>

//#include "gazebo.h"
#include "SubSimDriver.h"
#include "SimulationInterface.h"

//------------------------------------------------------------------------------
SimulationInterface::SimulationInterface( player_devaddr_t addr, 
    SubSimDriver* pDriver, ConfigFile* pConfigFile, int section )
    : SubSimInterface( addr, pDriver, pConfigFile, section )
{
}

//------------------------------------------------------------------------------
SimulationInterface::~SimulationInterface()
{
}

//------------------------------------------------------------------------------
// Handle all messages.
int SimulationInterface::ProcessMessage( QueuePointer& respQueue,
                                        player_msghdr_t* pHeader, void* pData )
{/*
    boost::recursive_mutex::scoped_lock lock(*this->mutex);
    if (this->responseQueue)
        delete this->responseQueue;

    this->responseQueue = new QueuePointer(respQueue);
*/
    // Set a 3D pose
    /*if ( Message::MatchMessage( pHeader, PLAYER_MSGTYPE_REQ,
        PLAYER_SIMULATION_REQ_SET_POSE3D, this->device_addr))
    {
        gazebo::SimulationRequestData *gzReq = NULL;
        player_simulation_pose3d_req_t* pRequestData =
            (player_simulation_pose3d_req_t*)(pData);

        this->iface->Lock(1);

        gzReq = &(this->iface->data->requests[ this->iface->data->requestCount++ ]);

        gzReq->type = gazebo::SimulationRequestData::SET_POSE3D;
        strcpy((char*)gzReq->modelName, req->name);

        gzReq->modelPose.pos.x = req->pose.px;
        gzReq->modelPose.pos.y = req->pose.py;
        gzReq->modelPose.pos.z = req->pose.pz;

        gzReq->modelPose.roll = req->pose.proll;
        gzReq->modelPose.pitch = req->pose.ppitch;
        gzReq->modelPose.yaw = req->pose.pyaw;

    this->iface->Unlock();

    this->driver->Publish(this->device_addr, respQueue,
                          PLAYER_MSGTYPE_RESP_ACK, 
                          PLAYER_SIMULATION_REQ_SET_POSE3D);

  }

  /// Set a 2D pose
  else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
                                 PLAYER_SIMULATION_REQ_SET_POSE2D, 
                                 this->device_addr))
  {
    gazebo::SimulationRequestData *gzReq = NULL;

    player_simulation_pose2d_req_t *req =
      (player_simulation_pose2d_req_t*)(data);

    this->iface->Lock(1);

    gzReq = &(this->iface->data->requests[ this->iface->data->requestCount++]);

    gzReq->type = gazebo::SimulationRequestData::SET_POSE2D;

    strcpy((char*)gzReq->modelName, req->name);

    gzReq->modelPose.pos.x = req->pose.px;
    gzReq->modelPose.pos.y = req->pose.py;
    gzReq->modelPose.yaw = req->pose.pa;

    this->iface->Unlock();

    this->driver->Publish(this->device_addr, respQueue,
                          PLAYER_MSGTYPE_RESP_ACK, 
                          PLAYER_SIMULATION_REQ_SET_POSE2D);
  }
*/
    // Get a 3d pose
    if ( Message::MatchMessage( pHeader, PLAYER_MSGTYPE_REQ,
        PLAYER_SIMULATION_REQ_GET_POSE3D, this->mDeviceAddress ) )
    {
        player_simulation_pose3d_req_t* pRequest =
            (player_simulation_pose3d_req_t*)(pData);

        Vector entityPos;
        Vector entityRotation;
        bool bEntityFound = mpDriver->mSim.GetEntityPose( 
            pRequest->name, &entityPos, &entityRotation );
        if ( bEntityFound )
        {
            pRequest->pose.px = entityPos.mX;
            pRequest->pose.py = entityPos.mY;
            pRequest->pose.pz = entityPos.mZ;

            pRequest->pose.ppitch = entityRotation.mX;
            pRequest->pose.proll = entityRotation.mY;
            pRequest->pose.pyaw = entityRotation.mZ;
            
            mpDriver->Publish( mDeviceAddress, respQueue, 
                PLAYER_MSGTYPE_RESP_ACK, PLAYER_SIMULATION_REQ_GET_POSE3D,
                pRequest, sizeof( player_simulation_pose3d_req_t ), NULL );
        }
        else
        {
            mpDriver->Publish( mDeviceAddress, respQueue, 
                PLAYER_MSGTYPE_RESP_NACK, PLAYER_SIMULATION_REQ_GET_POSE3D );
        }
        
        return 0;
    }
/*
  /// Get a 2D pose
  else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
                                 PLAYER_SIMULATION_REQ_GET_POSE2D, 
                                 this->device_addr))
  {
    gazebo::SimulationRequestData *gzReq = NULL;
    player_simulation_pose2d_req_t *req =
      (player_simulation_pose2d_req_t*)(data);

    this->iface->Lock(1);

    gzReq = &(this->iface->data->requests[this->iface->data->requestCount++]);

    gzReq->type = gazebo::SimulationRequestData::GET_POSE2D;

    strcpy((char*)gzReq->modelName, req->name);

    this->iface->Unlock();
  }
  else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_REQ,
                                 PLAYER_SIMULATION_REQ_GET_PROPERTY, 
                                 this->device_addr))
  {
    player_simulation_property_req_t *req =
      (player_simulation_property_req_t*)(data);

    std::string name = req->name;
    std::string prop = req->prop;

    if (name == "world")
    {
      this->iface->Lock(1);
      req->value = new char[ sizeof(double) ];
      req->value_count = sizeof(double);

      if (prop == "sim_time")
      {
        memcpy(req->value, &this->iface->data->simTime, sizeof(double));
      }
      else if (prop == "pause_time")
      {
        memcpy(req->value, &this->iface->data->pauseTime, sizeof(double));
      }
      else if (prop == "real_time")
      {
        memcpy(req->value, &this->iface->data->realTime, sizeof(double));
      }
      else if (prop == "state")
      {
        memcpy(req->value, &this->iface->data->state, sizeof(int));
      }
      this->iface->Unlock();

      this->driver->Publish(this->device_addr, respQueue,
                          PLAYER_MSGTYPE_RESP_ACK, PLAYER_SIMULATION_REQ_GET_PROPERTY, req, sizeof(*req), NULL);

      if (req->value)
      {
        delete [] req->value;
        req->value = NULL;
      }
    }
    else
    {
      this->iface->Lock(1);
      gazebo::SimulationRequestData *gzReq = NULL;
      gzReq = &(this->iface->data->requests[this->iface->data->requestCount++]);

      if (prop == "num_children")
      {
        gzReq->type = gazebo::SimulationRequestData::GET_NUM_CHILDREN;
        strcpy((char*)gzReq->modelName, req->name);   
      }
      else if (prop == "model_name")
      {
        gzReq->type = gazebo::SimulationRequestData::GET_MODEL_NAME;
        //gzReq->uintValue = req->index;
      }
      else if (prop == "child_name")
      {
        gzReq->type = gazebo::SimulationRequestData::GET_CHILD_NAME;
        //gzReq->uintValue = req->index;
        strcpy((char*)gzReq->modelName, req->name);   
      }
       else if (prop == "fiducial_id")
      {
        gzReq->type = gazebo::SimulationRequestData::GET_MODEL_FIDUCIAL_ID;
        strcpy((char*)gzReq->modelName, req->name);   
      }
      else if (prop == "model_type")
      {
        gzReq->type = gazebo::SimulationRequestData::GET_MODEL_TYPE;
        strcpy((char*)gzReq->modelName, req->name);   
      }
      else if ((prop == "num_models") && (name == "world"))
      { 
        gazebo::SimulationRequestData *gzReq = NULL;
        gzReq = &(this->iface->data->requests[this->iface->data->requestCount++]);
        gzReq->type = gazebo::SimulationRequestData::GET_NUM_MODELS;
      }
      else 
      {
        this->iface->data->requestCount--; //we did ++ but didnt introduced real request 
        std::cerr << "The object [" << name << "] does not have the property [" << prop << "].\n";
      }
      this->iface->Unlock();
    }
  }*/
  /*else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
                                 PLAYER_SIMULATION_CMD_PAUSE, 
                                 this->device_addr))
  {
    this->iface->Lock(1);
    gazebo::SimulationRequestData *gzReq = NULL;
    gzReq = &(this->iface->data->requests[this->iface->data->requestCount++]);
    gzReq->type = SimulationRequestData::PAUSE;
    this->iface->Unlock();
  } 
  else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
                                 PLAYER_SIMULATION_CMD_RESET, 
                                 this->device_addr))
  {
    this->iface->Lock(1);
    gazebo::SimulationRequestData *gzReq = NULL;
    gzReq = &(this->iface->data->requests[this->iface->data->requestCount++]);
    gzReq->type = SimulationRequestData::RESET;
    this->iface->Unlock();
  } 
  else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
                                 PLAYER_SIMULATION_CMD_SAVE, 
                                 this->device_addr))
  {
    this->iface->Lock(1);
    gazebo::SimulationRequestData *gzReq = NULL;
    gzReq = &(this->iface->data->requests[this->iface->data->requestCount++]);
    gzReq->type = SimulationRequestData::SAVE;
    this->iface->Unlock();
  }
  */
  //else
  //  printf("Unhandled Process message[%d][%d]\n",0,0);

    printf( "Unhandled message\n" );
    return -1;
}


//------------------------------------------------------------------------------
// Update this interface and publish new info.
void SimulationInterface::Update()
{
  /*boost::recursive_mutex::scoped_lock lock(*this->mutex);
  gazebo::SimulationRequestData *response = NULL;
  this->iface->Lock(1);

  for (unsigned int i=0; i < this->iface->data->responseCount; i++)
  {
    response = &(this->iface->data->responses[i]);

    switch (response->type)
    {
      case gazebo::SimulationRequestData::SET_STATE:
      case gazebo::SimulationRequestData::GO:
      case gazebo::SimulationRequestData::PAUSE:
      case gazebo::SimulationRequestData::RESET:
      case gazebo::SimulationRequestData::SAVE:
      case gazebo::SimulationRequestData::SET_POSE2D:
      case gazebo::SimulationRequestData::SET_POSE3D:
        break;

      case gazebo::SimulationRequestData::GET_POSE3D:
        {

          if (this->pose3dReq.name_count != strlen(response->modelName))
          {
            if (this->pose3dReq.name)
              delete [] this->pose3dReq.name;
            this->pose3dReq.name = new char[strlen(response->modelName)+1];
          }

          strcpy(this->pose3dReq.name, response->modelName);
          this->pose3dReq.name_count = strlen(this->pose3dReq.name);

          this->pose3dReq.pose.px = response->modelPose.pos.x;
          this->pose3dReq.pose.py = response->modelPose.pos.y;
          this->pose3dReq.pose.pz = response->modelPose.pos.z;

          this->pose3dReq.pose.proll = response->modelPose.roll;
          this->pose3dReq.pose.ppitch = response->modelPose.pitch;
          this->pose3dReq.pose.pyaw = response->modelPose.yaw;

          this->driver->Publish(this->device_addr, *(this->responseQueue),
              PLAYER_MSGTYPE_RESP_ACK, PLAYER_SIMULATION_REQ_GET_POSE3D,
              &this->pose3dReq, sizeof(this->pose3dReq), NULL);

          break;
        }
      case gazebo::SimulationRequestData::GET_POSE2D:
        {

          if (this->pose2dReq.name_count != strlen(response->modelName))
          {
            if (this->pose2dReq.name)
              delete [] this->pose2dReq.name;
            this->pose2dReq.name = new char[strlen(response->modelName)+1];
          }

          strcpy(this->pose2dReq.name, response->modelName);
          this->pose2dReq.name_count = strlen(this->pose2dReq.name);

          this->pose2dReq.pose.px = response->modelPose.pos.x;
          this->pose2dReq.pose.py = response->modelPose.pos.y;
          this->pose2dReq.pose.pa = response->modelPose.yaw;

          this->driver->Publish(this->device_addr, *(this->responseQueue),
              PLAYER_MSGTYPE_RESP_ACK, PLAYER_SIMULATION_REQ_GET_POSE2D,
              &this->pose2dReq, sizeof(this->pose2dReq), NULL);

          break;
        }

      case gazebo::SimulationRequestData::GET_MODEL_FIDUCIAL_ID:
        {
          player_simulation_property_req_t *req ;
          memset (req, 0, sizeof(player_simulation_property_req_t));

          memcpy(req->value, (const void *) response->uintValue, sizeof(response->uintValue));
          req->value_count = sizeof(response->uintValue);
          this->driver->Publish(this->device_addr, *(this->responseQueue),
                          PLAYER_MSGTYPE_RESP_ACK, PLAYER_SIMULATION_REQ_GET_PROPERTY, req, sizeof(*req), NULL);
          break;
        }




    }
  }

  this->iface->data->responseCount = 0;

  this->iface->Unlock();*/

  return;
}



