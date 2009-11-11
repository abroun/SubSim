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
};
