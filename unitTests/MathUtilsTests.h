//------------------------------------------------------------------------------
// File: MathUtilsTests.h
// Desc: Unit tests for the math utils
//
// Author: Alan Broun
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include <cxxtest/TestSuite.h>
#include "Common/MathUtils.h"

//------------------------------------------------------------------------------
class MathUtilsTests : public CxxTest::TestSuite 
{
    //--------------------------------------------------------------------------
    public: void testVectorTransformFromIrrlichtToSub()
    {
        irr::core::vector3df irrV1( 1.0f, 0.0f, 0.0f );
        irr::core::vector3df irrV2( 0.0f, 1.0f, 0.0f );
        irr::core::vector3df irrV3( 0.0f, 0.0f, 1.0f );
        irr::core::vector3df irrV4( 1.0f, 2.0f, 3.0f );

        Vector subV1 = MathUtils::TransformVector_IrrToSub( irrV1 );
        Vector subV2 = MathUtils::TransformVector_IrrToSub( irrV2 );
        Vector subV3 = MathUtils::TransformVector_IrrToSub( irrV3 );
        Vector subV4 = MathUtils::TransformVector_IrrToSub( irrV4 );
        
        TS_ASSERT( subV1 == Vector( 1.0f, 0.0f, 0.0f ) );
        TS_ASSERT( subV2 == Vector( 0.0f, 0.0f, 1.0f ) );
        TS_ASSERT( subV3 == Vector( 0.0f, 1.0f, 0.0f ) );
        TS_ASSERT( subV4 == Vector( 1.0f, 3.0f, 2.0f ) );
    }

    //--------------------------------------------------------------------------
    public: void testVectorTransformFromSubToIrrlicht()
    {
        Vector subV1( 1.0f, 0.0f, 0.0f );
        Vector subV2( 0.0f, 1.0f, 0.0f );
        Vector subV3( 0.0f, 0.0f, 1.0f );
        Vector subV4( 1.0f, 2.0f, 3.0f );

        irr::core::vector3df irrV1 = MathUtils::TransformVector_SubToIrr( subV1 );
        irr::core::vector3df irrV2 = MathUtils::TransformVector_SubToIrr( subV2 );
        irr::core::vector3df irrV3 = MathUtils::TransformVector_SubToIrr( subV3 );
        irr::core::vector3df irrV4 = MathUtils::TransformVector_SubToIrr( subV4 );
        
        TS_ASSERT( irrV1 == irr::core::vector3df( 1.0f, 0.0f, 0.0f ) );
        TS_ASSERT( irrV2 == irr::core::vector3df( 0.0f, 0.0f, 1.0f ) );
        TS_ASSERT( irrV3 == irr::core::vector3df( 0.0f, 1.0f, 0.0f ) );
        TS_ASSERT( irrV4 == irr::core::vector3df( 1.0f, 3.0f, 2.0f ) );
        
        Vector subCrossVector = subV1.CrossProduct( subV2 );
        irr::core::vector3df irrCrossVector = MathUtils::TransformVector_SubToIrr( subCrossVector );
        
        // This only works because pseudovectors are handled by our Vector class
        TS_ASSERT( irrCrossVector == irrV1.crossProduct( irrV2 ) );
    }
    
    //--------------------------------------------------------------------------
    public: void testRotationTransformFromIrrlichtToSub()
    {
        irr::core::vector3df irrRotation( 
            MathUtils::DegToRad( 45.0f ), MathUtils::DegToRad( -20.0f ), MathUtils::DegToRad( 175.0f ) );
        Vector subRotation = MathUtils::TransformRotation_IrrToSub( irrRotation );
        
        TS_ASSERT_DELTA( sinf( subRotation.mX ), sinf( MathUtils::DegToRad( -45.0f ) ), Common::DEFAULT_EPSILON );
        TS_ASSERT_DELTA( sinf( subRotation.mY ), sinf( MathUtils::DegToRad( -175.0f ) ), Common::DEFAULT_EPSILON );
        TS_ASSERT_DELTA( sinf( subRotation.mZ ), sinf( MathUtils::DegToRad( 20.0f ) ), Common::DEFAULT_EPSILON );
    }
    
    //--------------------------------------------------------------------------
    public: void testRotationTransformFromSubToIrrlicht()
    {
        Vector subRotation( 
            MathUtils::DegToRad( 45.0f ), MathUtils::DegToRad( -20.0f ), MathUtils::DegToRad( 175.0f ) );
        irr::core::vector3df irrRotation = MathUtils::TransformRotation_SubToIrr( subRotation );
        
        TS_ASSERT_DELTA( sinf( irrRotation.X ), sinf( MathUtils::DegToRad( -45.0f ) ), Common::DEFAULT_EPSILON );
        TS_ASSERT_DELTA( sinf( irrRotation.Y ), sinf( MathUtils::DegToRad( -175.0f ) ), Common::DEFAULT_EPSILON );
        TS_ASSERT_DELTA( sinf( irrRotation.Z ), sinf( MathUtils::DegToRad( 20.0f ) ), Common::DEFAULT_EPSILON );
    }
};
