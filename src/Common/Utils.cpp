//------------------------------------------------------------------------------
// File: Utils.h
// Desc: Common utility library for useful routines that have no other home
//
// Author: Alan Broun
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "Utils.h"
#include <strings.h>

//------------------------------------------------------------------------------
S32 Utils::stricmp( const char* s1, const char* s2 )
{
    return strcasecmp( s1, s2 );
}
