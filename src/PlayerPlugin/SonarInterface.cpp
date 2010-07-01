//------------------------------------------------------------------------------
// File: SonarInterface.cpp
// Desc: A very faked implementation of the Micron Sonar interface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "SonarInterface.h"

#include <math.h>
#include <stdio.h>
#include "SubSimDriver.h"

//------------------------------------------------------------------------------
SonarInterface::SonarInterface( player_devaddr_t addr, 
    SubSimDriver* pDriver, ConfigFile* pConfigFile, int section )
    : SubSimInterface( addr, pDriver, pConfigFile, section ),
    mbScanActive( false ),
    mScanStartAngle( 0.0f*M_PI/180.0f ),
    mScanEndAngle( 0.0f*M_PI/180.0f ),
    mRange( 5 ),
    mNumBins( 100 ),
    mGain( 0.1f )
{
}

//------------------------------------------------------------------------------
SonarInterface::~SonarInterface()
{
}

//------------------------------------------------------------------------------
// Handle all messages.
int SonarInterface::ProcessMessage( QueuePointer& respQueue,
                                        player_msghdr_t* pHeader, void* pData )
{
    if ( Message::MatchMessage( pHeader, PLAYER_MSGTYPE_CMD, 
        PLAYER_MICRONSONAR_CMD_SCAN, mDeviceAddress ) )
    {
        player_micronsonar_cmd_scan_t* pCmd = (player_micronsonar_cmd_scan_t*)pData;
        
        // Start a scan
        mScanStartAngle = pCmd->startAngle;
        mScanEndAngle = pCmd->endAngle;
        mbScanActive = true;
        mScanStartTime = HighPrecisionTime::GetTime();
    
        return 0;
    }
    else if( Message::MatchMessage( pHeader, PLAYER_MSGTYPE_REQ,
        PLAYER_MICRONSONAR_REQ_SET_CONFIG, mDeviceAddress ) )
    {
        player_micronsonar_config_t* pConfig =
            (player_micronsonar_config_t*)pData;

        mRange = pConfig->range;
        mNumBins = pConfig->numBins;
        mGain = pConfig->gain;
            
        mpDriver->Publish( mDeviceAddress, respQueue,
            PLAYER_MSGTYPE_RESP_ACK, PLAYER_MICRONSONAR_REQ_SET_CONFIG,
            pData, pHeader->size, NULL );

        return 0;
    }
    else if( Message::MatchMessage( pHeader, PLAYER_MSGTYPE_REQ,
        PLAYER_MICRONSONAR_REQ_GET_CONFIG, mDeviceAddress ) )
    {
        player_micronsonar_config_t config;
        config.range = mRange;
        config.numBins = mNumBins;
        config.gain = mGain;

        mpDriver->Publish( mDeviceAddress, respQueue,
            PLAYER_MSGTYPE_RESP_ACK, PLAYER_MICRONSONAR_REQ_GET_CONFIG,
            (void*)&config, sizeof(config), NULL );
    
        return 0;
    }
    
    printf( "Unhandled message\n" );
    return -1;
}


//------------------------------------------------------------------------------
// Update this interface and publish new info.
void SonarInterface::Update()
{
    HighPrecisionTime curTime = HighPrecisionTime::GetTime();
    
    F32 timeDiff = HighPrecisionTime::ConvertToSeconds(
        HighPrecisionTime::GetDiff( curTime, mScanStartTime ) );
    
    if ( mbScanActive 
        && timeDiff > 5.0f )
    {
        mbScanActive = false;
     
        // Build up the data struct
        player_micronsonar_data_t data;
        data.range = mRange;
        data.numBins = mNumBins;
        data.startAngle = mScanStartAngle;
        data.endAngle = mScanEndAngle;
        
        S32 imageDim = 2*mRange;
                
        data.centreX = imageDim/2;
        data.centreY = imageDim/2;
        data.width = imageDim;
        data.height = imageDim;
        data.bpp = 8;
        data.format = PLAYER_MICRONSONAR_FORMAT_MONO8;
        data.image_count = data.width*data.height;
        data.image = new U8[ data.image_count ];
        memset( data.image, 0, data.image_count );    // Clear image
    
        // Put something vaguely interesting in
        for ( S32 y = 0; y < data.height; y++ )
        {
            for ( S32 x = 0; x < data.width; x++ )
            {
                data.image[ data.width*y + x ] = (x+y)%256;
            }
        }
                
        // Write data to the client (through the server)
        mpDriver->Publish( mDeviceAddress,
            PLAYER_MSGTYPE_DATA, PLAYER_MICRONSONAR_DATA_STATE, &data );
        delete [] data.image;
    
    }
}



