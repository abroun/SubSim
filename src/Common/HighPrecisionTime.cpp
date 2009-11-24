//------------------------------------------------------------------------------
// File: HighPrecisionTime.cpp
// Desc: Rough and ready way of getting high precision time values in a cross
//       platform way
//
// Author: Alan Broun
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "HighPrecisionTime.h"

#include <time.h>
#include <stdio.h>

//------------------------------------------------------------------------------
// HighPrecisionTime
//------------------------------------------------------------------------------
HighPrecisionTime HighPrecisionTime::GetTime()
{
    timespec timeSpec;
    clock_gettime( CLOCK_REALTIME, &timeSpec );
    
    HighPrecisionTime result;
    result.mSeconds = (S32)timeSpec.tv_sec;
    result.mNanoSeconds = (S32)timeSpec.tv_nsec;
    
    return result;
}
    
//------------------------------------------------------------------------------
HighPrecisionTime HighPrecisionTime::GetDiff( const HighPrecisionTime& end, 
                                              const HighPrecisionTime& start )
{
    HighPrecisionTime result;
    
    if ( end.mNanoSeconds < start.mNanoSeconds ) 
    {
        result.mSeconds = end.mSeconds - start.mSeconds - 1;
        result.mNanoSeconds = 1000000000 + end.mNanoSeconds - start.mNanoSeconds;
    } 
    else 
    {
        result.mSeconds = end.mSeconds - start.mSeconds;
        result.mNanoSeconds = end.mNanoSeconds - start.mNanoSeconds;
    }
    return result;
}
    
//------------------------------------------------------------------------------
S32 HighPrecisionTime::ConvertToMilliSeconds( const HighPrecisionTime& t )
{
    return (t.mSeconds*1000) + (t.mNanoSeconds/1000000);
}

//------------------------------------------------------------------------------
S32 HighPrecisionTime::ConvertToMicroSeconds( const HighPrecisionTime& t )
{
    return (t.mSeconds*1000000) + (t.mNanoSeconds/1000);
}