//------------------------------------------------------------------------------
// File: Common.h
// Desc: Common typedefs and defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef COMMON_H
#define COMMON_H

//------------------------------------------------------------------------------
typedef int S32;
typedef unsigned int U32;
typedef short S16;
typedef unsigned short U16;
typedef char S8;
typedef unsigned char U8;

typedef float F32;

//------------------------------------------------------------------------------
class Common
{
    public: static const F32 DEFAULT_EPSILON = 0.00001f;
};

#endif // _COMMON_H
