//------------------------------------------------------------------------------
// File: MathUtils.cpp
// Desc: Hold all utility library that may need to be re-organised at some
//       point
//
// Author: Alan Broun
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "MathUtils.h"

//------------------------------------------------------------------------------
Vector MathUtils::TransformVector_IrrToSub( const irr::core::vector3df& irrVector )
{
    // This should be equivilant to flipping the y-axis and then rotating 90
    // degrees around the x-axis
    return Vector( irrVector.X, irrVector.Z, irrVector.Y );
}

//------------------------------------------------------------------------------
irr::core::vector3df MathUtils::TransformVector_SubToIrr( const Vector& subVector )
{
}
