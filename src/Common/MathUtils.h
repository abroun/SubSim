//------------------------------------------------------------------------------
// File: MathUtils.h
// Desc: Hold all utility library that may need to be re-organised at some
//       point
//
// Author: Alan Broun
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

//------------------------------------------------------------------------------
#include <math.h>
#include <irrlicht/irrlicht.h>
#include "Vector.h"

//------------------------------------------------------------------------------
class MathUtils
{
    //--------------------------------------------------------------------------
    public: static F32 RadToDeg( F32 radians ) 
    { 
        return radians * 180.0f / M_PI;
    }
    
    //--------------------------------------------------------------------------
    public: static F32 DegToRad( F32 degrees ) 
    { 
        return degrees * M_PI / 180.0f;
    }
    
    //--------------------------------------------------------------------------
    public: static Vector TransformVector_IrrToSub( const irr::core::vector3df& irrVector );
    public: static irr::core::vector3df TransformVector_SubToIrr( const Vector& subVector );
    
    //--------------------------------------------------------------------------
    // Takes a vector of rotations around the (x,y,z) axes and transforms them
    // to the appropriate coordinate system
    public: static Vector TransformRotation_IrrToSub( const irr::core::vector3df& irrRotationVector );
    public: static irr::core::vector3df TransformRotation_SubToIrr( const Vector& subRotationVector );
};

#endif // MATH_UTILS_H
