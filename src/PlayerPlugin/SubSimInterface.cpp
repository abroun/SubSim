//------------------------------------------------------------------------------
// File: SubSimInterface.h
// Desc: Generic SuSim device inteface for Player
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "SubSimInterface.h"
#include "SubSimDriver.h"

//------------------------------------------------------------------------------
SubSimInterface::SubSimInterface( player_devaddr_t addr, SubSimDriver* pDriver,
                                 ConfigFile* /* pConfigFile */, int /*section*/ )
{
    mDeviceAddress = addr;
    mpDriver = pDriver;
}

//------------------------------------------------------------------------------
SubSimInterface::~SubSimInterface()
{
}

