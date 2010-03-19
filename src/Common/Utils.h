//------------------------------------------------------------------------------
// File: Utils.h
// Desc: Common utility library for useful routines that have no other home
//
// Author: Alan Broun
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef UTILS_H
#define UTILS_H

//------------------------------------------------------------------------------
#include "Common.h"

//------------------------------------------------------------------------------
class Utils
{
    //--------------------------------------------------------------------------
    // Apparently Linux does not have stricmp so we provide a cross platform
    // alternative
    public: static S32 stricmp( const char* s1, const char* s2 );
};

#endif // UTILS_H
