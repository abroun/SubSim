//------------------------------------------------------------------------------
// File: SonarInterface.h
// Desc: A very faked implementation of the Micron Sonar interface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef SONAR_INTERFACE_H
#define SONAR_INTERFACE_H

//------------------------------------------------------------------------------
#include "SubSimInterface.h"
#include "Common/HighPrecisionTime.h"

//------------------------------------------------------------------------------
class SonarInterface : public SubSimInterface
{
    // Constructor
    public: SonarInterface( player_devaddr_t addr, SubSimDriver* pDriver,
                            ConfigFile* pConfigFile, int section );
    // Destructor
    public: virtual ~SonarInterface();

    // Handle all messages.
    public: virtual int ProcessMessage( QueuePointer &respQueue,
                                      player_msghdr_t* pHeader, void* pData );

    // Update this interface, publish new info.
    public: virtual void Update();
    
    private: HighPrecisionTime mScanStartTime;
    private: bool mbScanActive;
    private: F32 mScanStartAngle;
    private: F32 mScanEndAngle;
    private: S32 mRange;
    private: S32 mNumBins;
    private: F32 mGain;
};

#endif // SONAR_INTERFACE_H
