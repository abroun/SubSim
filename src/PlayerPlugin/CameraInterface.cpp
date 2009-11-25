//------------------------------------------------------------------------------
// File: CameraInterface.cpp
// Desc: Provides a view from a camera in the SubSim world
//-------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "CameraInterface.h"

#include <stdio.h>
#include "SubSimDriver.h"

//------------------------------------------------------------------------------
CameraInterface::CameraInterface( player_devaddr_t addr, 
    SubSimDriver* pDriver, ConfigFile* pConfigFile, int section )
    : SubSimInterface( addr, pDriver, pConfigFile, section )
{
    mpDriver->mSim.GetSubCameraImageDimensions( &mImageWidth, &mImageHeight );
    mImageBufferSize = mImageWidth*mImageHeight*3;
    mpImageData = new U8[ mImageBufferSize ];
}

//------------------------------------------------------------------------------
CameraInterface::~CameraInterface()
{
    if ( NULL != mpImageData )
    {
        delete [] mpImageData;
        mpImageData = NULL;
    }
}

//------------------------------------------------------------------------------
// Handle all messages.
int CameraInterface::ProcessMessage( QueuePointer& respQueue,
                                        player_msghdr_t* pHeader, void* pData )
{
    // No messages for the camera interface
    return -1;
}


//------------------------------------------------------------------------------
// Update this interface and publish new info.
void CameraInterface::Update()
{
    player_camera_data_t data;
    
    // Get the camera image and then return it
    mpDriver->mSim.GetSubCameraImage( mpImageData, mImageBufferSize );
    mImageTimestamp = mpDriver->mSim.GetSimTime();
    
    data.width = mImageWidth;
    data.height = mImageHeight;
    data.bpp = 24;
    data.format = PLAYER_CAMERA_FORMAT_RGB888;
    data.fdiv = 1;
    data.compression = PLAYER_CAMERA_COMPRESS_RAW;
    data.image_count = mImageBufferSize;
    data.image = mpImageData;

    mpDriver->Publish( this->mDeviceAddress,
                       PLAYER_MSGTYPE_DATA, PLAYER_CAMERA_DATA_STATE,
                       (void*)&data, sizeof( data ), &mImageTimestamp );
          
    /*FILE* pPPMFile = fopen( "Camera.ppm", "w" );
    fprintf( pPPMFile, "P3\n" );
    fprintf( pPPMFile, "%i %i\n", mImageWidth, mImageHeight );
    fprintf( pPPMFile, "255\n" );
    
    U8* pPixelData = mpImageData;
    for (
    
    fopen(*/
}



