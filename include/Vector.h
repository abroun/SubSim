//------------------------------------------------------------------------------
// File: Vector.h
// Desc: A 3D vector for use in the simulator.
//
// Note: This vector keeps track of whether it is a pseudovector or not. A
//       vector produced by the cross product may be a pseudovector. If a
//       pseudovector is subject to an improper rotation (i.e. due to a 
//       transformation from a Left Handed coordinate system to a Right Handed
//       coordinate system) then it is first scaled by -1.0f.
//
//       See http://en.wikipedia.org/wiki/Cross_product#Cross_product_and_handedness
//       for further details
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef VECTOR_H
#define VECTOR_H

//------------------------------------------------------------------------------
#include "Common.h"
#include <math.h>

//------------------------------------------------------------------------------
class Vector
{
    //--------------------------------------------------------------------------
    public: Vector() {}
    public: Vector( F32 x, F32 y, F32 z, bool bIsPseudoVector = false )
        : mX( x ), mY( y ), mZ( z ), mbIsPseudoVector( bIsPseudoVector ) {}
    public: Vector( const Vector& v )
        : mX( v.mX ), mY( v.mY ), mZ( v.mZ ), mbIsPseudoVector( v.mbIsPseudoVector ) {}

    //--------------------------------------------------------------------------
    // Operators
    public: Vector operator-() const 
    { 
        return Vector( -mX, -mY, -mZ, mbIsPseudoVector ); 
    }
    
    public: Vector& operator=( const Vector& v ) 
    { 
        mX = v.mX; mY = v.mY; mZ = v.mZ; mbIsPseudoVector = v.mbIsPseudoVector;
        return *this; 
    }

    public: Vector operator+( const Vector& v ) const 
    { 
        return Vector( mX + v.mX, mY + v.mY, mZ + v.mZ, mbIsPseudoVector ); 
    }

    public: Vector& operator+=( const Vector& v ) 
    { 
        mX += v.mX; mY += v.mY; mZ += v.mZ; 
        return *this; 
    }

    public: Vector operator-( const Vector& v ) const 
    { 
        return Vector( mX - v.mX, mY - v.mY, mZ - v.mZ, mbIsPseudoVector ); 
    }

    public: Vector& operator-=( const Vector& v ) 
    { 
        mX -= v.mX; mY -= v.mY; mZ -= v.mZ; 
        return *this; 
    }
 
    public: Vector operator*( F32 s ) const 
    { 
        return Vector( mX * s, mY * s, mZ * s, mbIsPseudoVector ); 
    }

    public: Vector& operator*=( F32 s ) 
    { 
        mX *= s; mY *= s; mZ *= s; 
        return *this; 
    }
 
    public: Vector operator/( F32 s ) const 
    { 
        F32 scale = 1.0 / s; 
        return Vector( mX * scale, mY * scale, mZ * scale, mbIsPseudoVector ); 
    }

    public: Vector& operator/=( F32 s ) 
    { 
        F32 scale = 1.0 / s;
        return ((*this) *= scale); 
    }
  
    public: bool operator==( const Vector& v ) const
    {
        return Equals( v );
    } 

    public: bool operator!=( const Vector& v ) const
    {
        return !Equals( v );
    }
    
    //--------------------------------------------------------------------------
    // Functions
    public: bool Equals( const Vector& v, F32 tolerance = Common::DEFAULT_EPSILON ) const
    {        
        // Ignores 'mbIsPseudoVector'
        return ( fabsf( v.mX - mX ) <= tolerance
            && fabsf( v.mY - mY ) <= tolerance
            && fabsf( v.mZ - mZ ) <= tolerance );
    }

    public: bool PseudoEquals( const Vector& v, F32 tolerance = Common::DEFAULT_EPSILON ) const
    {        
        return ( fabsf( v.mX - mX ) <= tolerance
            && fabsf( v.mY - mY ) <= tolerance
            && fabsf( v.mZ - mZ ) <= tolerance
            && mbIsPseudoVector == v.mbIsPseudoVector );
    }
 
    public: Vector& Set( F32 x, F32 y, F32 z, bool bIsPseudoVector = false ) 
    {
        mX = x; mY = y; mZ = z; mbIsPseudoVector = bIsPseudoVector;
        return *this;
    }
 
    public: F32 GetLength() const 
    { 
        return sqrtf( GetLengthSquared() ); 
    }

    public: F32 GetLengthSquared() const 
    { 
        return mX*mX + mY*mY + mZ*mZ; 
    }
 
    public: F32 DotProduct( const Vector& v ) const
    {
        return mX*v.mX + mY*v.mY + mZ*v.mZ;
    }
 
    public: Vector CrossProduct( const Vector& v ) const
    {
        bool bNewVectorIsPseudoVector;
        
        if ( !mbIsPseudoVector )
        {
            bNewVectorIsPseudoVector = !v.mbIsPseudoVector;
        }
        else
        {
            bNewVectorIsPseudoVector = v.mbIsPseudoVector;
        }
        
        return Vector( mY * v.mZ - mZ * v.mY, 
            mZ * v.mX - mX * v.mZ, mX * v.mY - mY * v.mX, bNewVectorIsPseudoVector );
    }
 
    public: Vector& Normalise()
    {
        F32 lengthSquared = GetLengthSquared();
        if ( lengthSquared > 0.0f )
        {
            F32 scale = 1.0 / sqrt( lengthSquared );
            mX *= scale;
            mY *= scale;
            mZ *= scale;
        }

        return *this;
    }       

    //--------------------------------------------------------------------------
    // Variables
    public: F32 mX, mY, mZ;
    public: bool mbIsPseudoVector;
};

//------------------------------------------------------------------------------
inline Vector operator*( F32 s, const Vector& v ) { return v*s; }

#endif // VECTOR_H

