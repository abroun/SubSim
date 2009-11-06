//------------------------------------------------------------------------------
// File: Vector.h
// Desc: A simple 3D vector for use in the simulator
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef VECTOR_H
#define VECTOR_H

//------------------------------------------------------------------------------
#include "Common.h"

//------------------------------------------------------------------------------
class Vector
{
    //--------------------------------------------------------------------------
    public: Vector() {}
    public: Vector( F32 x, F32 y, F32 z, F32 w = 1.0f )
        : mX( x ), mY( y ), mZ( z ), mW( w ) {}
    public: Vector( const Vector& v )
        : mX( v.mX ), mY( v.mY ), mZ( v.mZ ), mW( v.mW ) {}

    //--------------------------------------------------------------------------
    // Operators
    public: Vector operator-() const 
    { 
        return Vector( -mX, -mY, -mZ ); 
    }
    
    public: Vector& operator=( const Vector& v ) 
    { 
        mX = v.mX; mY = v.mY; mZ = v.mZ; 
        return *this; 
    }

    public: Vector operator+( const Vector& v ) const 
    { 
        return Vector( mX + v.mX, mY + v.mY, mZ + v.mZ ); 
    }

    public: Vector& operator+=( const Vector& v ) 
    { 
        mX += v.mX; mY += v.mY; mZ += v.mZ; 
        return *this; 
    }

    public: Vector operator-( const Vector& v ) const 
    { 
        return Vector( mX - v.mX, mY - v.mY, mZ - v.mZ ); 
    }

    public: Vector& operator-=( const Vector& v ) 
    { 
        mX -= v.mX; mY -= v.mY; mZ -= v.mZ; 
        return *this; 
    }
 
    public: Vector operator*( F32 s ) const 
    { 
        return Vector( mX * s, mY * s, mZ * s ); 
    }

    public: Vector& operator*=( F32 s ) 
    { 
        mX *= s; mY *= s; mZ *= s; 
        return *this; 
    }
 
    public: Vector operator/( F32 s ) const 
    { 
        F32 scale = 1.0 / s; 
        return Vector( mX * scale, mY * scale, mZ * scale ); 
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
    public: bool Equals( const Vector& v, F32 tolerance = DEFAULT_TOLERANCE ) const
    {
        return ( fabsf( v.mX - mX ) <= tolerance
            && fabsf( v.mY - mY ) <= tolerance
            && fabsf( v.mZ - mZ ) <= tolerance );
    }
 
    public: Vector& Set( F32 x, F32 y, F32 z ) 
    {
        mX = x; mY = y; mZ = z; 
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
 
    public: Vector CrossProductLeftHanded( const Vector& v ) const
    {
        return Vector( mY * v.mZ - mZ * v.mY, 
            mZ * v.mX - mX * v.mZ, mX * v.mY - mY * v.mX );
    }
 
    public: Vector& Normalize()
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

    public: static const F32 DEFAULT_TOLERANCE = 0.0001f;

    //--------------------------------------------------------------------------
    // Variables
    private: F32 mX, mY, mZ, mW;
};

//------------------------------------------------------------------------------
Vector operator*( F32 s, const Vector& v ) { return v*s; }

#endif // VECTOR_H

