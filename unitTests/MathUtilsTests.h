// MyTestSuite.h
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
    }
};
