//------------------------------------------------------------------------------
// File: VectorTests.h
// Desc: Unit tests for the vector class
//
// Author: Alan Broun
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include <cxxtest/TestSuite.h>
#include "Vector.h"

//------------------------------------------------------------------------------
class VectorTests : public CxxTest::TestSuite 
{
    //--------------------------------------------------------------------------
    public: void testConstructor()
    {
        Vector v1( 1.0f, 2.0f, 4.0f, true );
        Vector v2( v1 );
        
        TS_ASSERT( v1.mX == 1.0f );
        TS_ASSERT( v1.mY == 2.0f );
        TS_ASSERT( v1.mZ == 4.0f );
        TS_ASSERT( v1.mbIsPseudoVector == true );
        TS_ASSERT( v2.mX == v1.mX );
        TS_ASSERT( v2.mY == v1.mY );
        TS_ASSERT( v2.mZ == v1.mZ );
        TS_ASSERT( v1.mbIsPseudoVector == true );
    }
    
    //--------------------------------------------------------------------------
    public: void testAssignment()
    {
        Vector v1( 1.0f, 2.0f, 4.0f );
        
        Vector v2 = v1;
        TS_ASSERT( v2.mX == 1.0f );
        TS_ASSERT( v2.mY == 2.0f );
        TS_ASSERT( v2.mZ == 4.0f );
        TS_ASSERT( v2.mbIsPseudoVector == false );
    }
    
    //--------------------------------------------------------------------------
    public: void testAddition()
    {
        Vector v1( 1.0f, 2.0f, 4.0f );
        Vector v2( 3.0f, 5.0f, 0.0f, true );

        Vector vOut = v1 + v2;
        TS_ASSERT( vOut.mX == 4.0f );
        TS_ASSERT( vOut.mY == 7.0f );
        TS_ASSERT( vOut.mZ == 4.0f );
        TS_ASSERT( vOut.mbIsPseudoVector == false );
        
        vOut = v2;
        vOut += v1;
        TS_ASSERT( vOut.mX == 4.0f );
        TS_ASSERT( vOut.mY == 7.0f );
        TS_ASSERT( vOut.mZ == 4.0f );
        TS_ASSERT( vOut.mbIsPseudoVector == true );
    } 
    
    //--------------------------------------------------------------------------
    public: void testSubtraction()
    {
        Vector v1( 1.0f, 2.0f, 4.0f, true );
        Vector v2( 3.0f, 5.0f, 0.0f );

        Vector vOut = v1 - v2;
        TS_ASSERT( vOut.mX == -2.0f );
        TS_ASSERT( vOut.mY == -3.0f );
        TS_ASSERT( vOut.mZ == 4.0f );
        TS_ASSERT( vOut.mbIsPseudoVector == true );
        
        vOut -= v1;
        TS_ASSERT( vOut.mX == -3.0f );
        TS_ASSERT( vOut.mY == -5.0f );
        TS_ASSERT( vOut.mZ == 0.0f );
        TS_ASSERT( vOut.mbIsPseudoVector == true );
        
        vOut = -vOut;
        TS_ASSERT( vOut.mX == 3.0f );
        TS_ASSERT( vOut.mY == 5.0f );
        TS_ASSERT( vOut.mZ == 0.0f );
        TS_ASSERT( vOut.mbIsPseudoVector == true );
    }
    
    //--------------------------------------------------------------------------
    public: void testMultiplication()
    {
        Vector v( 1.0f, 2.0f, 4.0f, true );

        Vector vOut = v*5;
        TS_ASSERT( vOut.mX == 5.0f );
        TS_ASSERT( vOut.mY == 10.0f );
        TS_ASSERT( vOut.mZ == 20.0f );
        TS_ASSERT( vOut.mbIsPseudoVector == true );
        
        vOut = 4.0f * v;
        TS_ASSERT( vOut.mX == 4.0f );
        TS_ASSERT( vOut.mY == 8.0f );
        TS_ASSERT( vOut.mZ == 16.0f );
        TS_ASSERT( vOut.mbIsPseudoVector == true );
        
        vOut *= 0.5;
        TS_ASSERT( vOut.mX == 2.0f );
        TS_ASSERT( vOut.mY == 4.0f );
        TS_ASSERT( vOut.mZ == 8.0f );
        TS_ASSERT( vOut.mbIsPseudoVector == true );
    }
    
    //--------------------------------------------------------------------------
    public: void testDivision()
    {
        Vector v( 1.0f, 2.0f, 4.0f, true );

        Vector vOut = v / 2.0f;
        TS_ASSERT( vOut.mX == 0.5f );
        TS_ASSERT( vOut.mY == 1.0f );
        TS_ASSERT( vOut.mZ == 2.0f );
        TS_ASSERT( vOut.mbIsPseudoVector == true );
        
        vOut.mbIsPseudoVector = false;
        vOut /= 0.25f;
        TS_ASSERT( vOut.mX == 2.0f );
        TS_ASSERT( vOut.mY == 4.0f );
        TS_ASSERT( vOut.mZ == 8.0f );
        TS_ASSERT( vOut.mbIsPseudoVector == false );
    }
    
    //--------------------------------------------------------------------------
    public: void testEquality()
    {
        Vector v1( 1.0f, 2.0f, 4.0f );
        Vector v2( 1.0f, 2.0f, 4.0f + Common::DEFAULT_EPSILON / 2.0f );
        Vector v3( 11.0f, 2.0f, 9.0f );
        Vector v4( 1.0f, 2.0f, 4.0f, true );
        
        TS_ASSERT( v1 == v2 );
        TS_ASSERT( v1 != v3 );
        TS_ASSERT( v3.Equals( v1, 10.0f ) );
        TS_ASSERT( !v1.PseudoEquals( v4 ) );
    }
    
    //--------------------------------------------------------------------------
    public: void testSet()
    {
        Vector v( 1.0f, 2.0f, 4.0f, true );
        
        v.Set( 3.0f, 9.0f, 6.0f );
        TS_ASSERT( v.mX == 3.0f );
        TS_ASSERT( v.mY == 9.0f );
        TS_ASSERT( v.mZ == 6.0f );
        TS_ASSERT( v.mbIsPseudoVector == false );
    }
    
    //--------------------------------------------------------------------------
    public: void testGetLength()
    {
        Vector v( 3.0f, 4.0f, 0.0f );
        
        TS_ASSERT_DELTA( v.GetLength(), 5.0f, Common::DEFAULT_EPSILON );
    }
    
    //--------------------------------------------------------------------------
    public: void testGetLengthSquared()
    {
        Vector v( 3.0f, 0.0f, 4.0f );
        
        TS_ASSERT_DELTA( v.GetLengthSquared(), 25.0f, Common::DEFAULT_EPSILON );
    }

    //--------------------------------------------------------------------------
    public: void testDotProduct()
    {
        Vector v1( 3.0f, 4.0f, 1.0f, true );
        Vector v2( 3.0f, 0.0f, 5.0f );
        
        TS_ASSERT_DELTA( v1.DotProduct( v2 ), 14.0f, Common::DEFAULT_EPSILON );
    }
    
    //--------------------------------------------------------------------------
    public: void testCrossProduct()
    {
        Vector vX( 1.0f, 0.0f, 0.0f );
        Vector vY( 0.0f, 1.0f, 0.0f );
        Vector vZ( 0.0f, 0.0f, 1.0f );
        
        TS_ASSERT( vZ.CrossProduct( vX ) == vY );
        TS_ASSERT( vX.CrossProduct( vY ) == vZ );
        TS_ASSERT( vY.CrossProduct( vZ ) == vX );
        
        TS_ASSERT( vX.CrossProduct( vZ ) == -vY );
        TS_ASSERT( vY.CrossProduct( vX ) == -vZ );
        TS_ASSERT( vZ.CrossProduct( vY ) == -vX );
    }
 
    //--------------------------------------------------------------------------
    public: void testNormalise()
    {
        Vector v( 3.0f, 4.0f, 9.0f, true );
        
        v.Normalise();
        
        TS_ASSERT_DELTA( v.GetLength(), 1.0f, Common::DEFAULT_EPSILON );
        TS_ASSERT_DELTA( v.GetLengthSquared(), 1.0f, Common::DEFAULT_EPSILON );
        TS_ASSERT( v.mbIsPseudoVector == true );
    }
    
};

