//------------------------------------------------------------------------------
// File: HighPrecisionTime.h
// Desc: Rough and ready way of getting high precision time values in a cross
//       platform way
//
// Author: Alan Broun
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef HIGH_PRECISION_TIME_H
#define HIGH_PRECISION_TIME_H

//------------------------------------------------------------------------------
#include "Common.h"

//------------------------------------------------------------------------------
class HighPrecisionTime
{
    public: S32 mSeconds;       // Number of elapsed seconds
    public: S32 mNanoSeconds;   // The rest of the time expressed as nano-seconds
    
    //--------------------------------------------------------------------------
    // Gets the current value of the timer
    public: static HighPrecisionTime GetTime();
    
    //--------------------------------------------------------------------------
    // Works out end - start
    public: static HighPrecisionTime GetDiff( const HighPrecisionTime& end, 
                                              const HighPrecisionTime& start );
    
    //--------------------------------------------------------------------------
    // Converts a time to milliseconds. This routine is mainly meant to be used
    // with time differences. Therefore beware of overflow.
    public: static S32 ConvertToMilliSeconds( const HighPrecisionTime& t );
    
    //--------------------------------------------------------------------------
    // Converts a time to microseconds. This routine is mainly meant to be used
    // with time differences. Therefore beware of overflow.
    public: static S32 ConvertToMicroSeconds( const HighPrecisionTime& t );
};

#endif // HIGH_PRECISION_TIME_H
