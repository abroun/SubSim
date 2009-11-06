// MyTestSuite.h
#include <cxxtest/TestSuite.h>
#include "Vector.h"

class VectorTests : public CxxTest::TestSuite 
{
public:
    void testVectorAddition()
    {
        Vector v1( 1.0f, 2.0f, 4.0f );
        Vector v2( 3.0f, 5.0f, 0.0f, 3.0f );

        Vector vOut = v1 + v2;
        TS_ASSERT( vOut.mX == 4.0f );
        TS_ASSERT( vOut.mY == 7.0f );
        TS_ASSERT( vOut.mZ == 4.0f );
        TS_ASSERT( vOut.mW == 1.0f );
    }    
};

