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
    irr::core::vector3df irrVector( subVector.mX, subVector.mZ, subVector.mY );
    
    if ( subVector.mbIsPseudoVector )
    {
        // Pseudo vectors need to be inverted
        irrVector *= -1.0f;
    }   

    return irrVector;
}

//------------------------------------------------------------------------------
Vector MathUtils::TransformRotation_IrrToSub( const irr::core::vector3df& irrRotationVector )
{
    return Vector( 2.0f*M_PI - irrRotationVector.X,
                   2.0f*M_PI - irrRotationVector.Z,
                   2.0f*M_PI - irrRotationVector.Y );
}

//------------------------------------------------------------------------------
irr::core::vector3df MathUtils::TransformRotation_SubToIrr( const Vector& subRotationVector )
{
    return irr::core::vector3df( 
        2.0f*M_PI - subRotationVector.mX,
        2.0f*M_PI - subRotationVector.mZ,
        2.0f*M_PI - subRotationVector.mY );
}
