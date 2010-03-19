//------------------------------------------------------------------------------
// File: CommandLineParserTests.h
// Desc: Unit tests for the command line parser
//
// Author: Alan Broun
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include <cxxtest/TestSuite.h>
#include "Common/CommandLineParser.h"
#include "Common/Utils.h"

//------------------------------------------------------------------------------
class CommandLineParserTests : public CxxTest::TestSuite 
{
    //--------------------------------------------------------------------------
    public: void testParsing()
    {
        const char* argc[] =
        {
            "programName",
            "-bunny=rabbit",
            "-frog",
            "-dog="
        };
        
        CommandLineParser::ParseCommandLine( 4, argc );
        
        TS_ASSERT( CommandLineParser::IsArgSet( "bunny" ) );
        TS_ASSERT( CommandLineParser::IsArgSet( "buNnY" ) );
        TS_ASSERT( CommandLineParser::IsArgSet( "frog" ) );
        TS_ASSERT( CommandLineParser::IsArgSet( "toad" ) == false );
        
        TS_ASSERT( Utils::stricmp( CommandLineParser::GetArgValue( "bunny" ), "rabbit" ) == 0 );
        TS_ASSERT( CommandLineParser::GetArgValue( "frog" ) == NULL );
        TS_ASSERT( CommandLineParser::GetArgValue( "dog" ) == NULL );
    }
};