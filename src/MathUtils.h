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
#include <irrlicht/irrlicht.h>
#include "Vector.h"

//------------------------------------------------------------------------------
class MathUtils
{
    public: static Vector TransformVector_IrrToSub( const irr::core::vector3df& irrVector );
    public: static irr::core::vector3df TransformVector_SubToIrr( const Vector& subVector );
};

#endif // MATH_UTILS_H
