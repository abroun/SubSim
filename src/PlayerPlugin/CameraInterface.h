//------------------------------------------------------------------------------
// File: CameraInterface.h
// Desc: Provides a view from a camera in the SubSim world
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef CAMERA_INTERFACE_H
#define CAMERA_INTERFACE_H

//------------------------------------------------------------------------------
#include "Common.h"
#include "SubSimInterface.h"

//------------------------------------------------------------------------------
class CameraInterface : public SubSimInterface
{
    // Constructor
    public: CameraInterface( player_devaddr_t addr, SubSimDriver* pDriver,
                              ConfigFile* pConfigFile, int section );
    // Destructor
    public: virtual ~CameraInterface();

    // Handle all messages.
    public: virtual int ProcessMessage( QueuePointer &respQueue,
                                      player_msghdr_t* pHeader, void* pData );

    // Update this interface, publish new info.
    public: virtual void Update();
    
    private: U8* mpImageData;
    private: U32 mImageWidth;
    private: U32 mImageHeight;
    private: U32 mImageBufferSize;
    private: double mImageTimestamp;
};

#endif // CAMERA_INTERFACE_H