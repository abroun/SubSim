//------------------------------------------------------------------------------
// A simple simulator for the UWE group project
//------------------------------------------------------------------------------
#include <stdio.h>

#include "Simulator/Simulator.h"
#include "Common/CommandLineParser.h"

//------------------------------------------------------------------------------
void ShowUsage( const char* programName );

//------------------------------------------------------------------------------
int main( int argc, const char** argv )
{   
    CommandLineParser::ParseCommandLine( argc, argv );
    if ( CommandLineParser::IsArgSet( "h" ) )
    {
        ShowUsage( argv[ 0 ] );
        return 0;
    }
    
    Simulator sim;
    
    if ( !sim.Init( CommandLineParser::GetArgValue( "world" ) ) )
    {
        fprintf( stderr, "Error: Unable to initialise simulator\n" );
        return -1;
    }
    
    while ( sim.IsRunning() )
    {
        sim.Update();
    }

    return 0;
}

//------------------------------------------------------------------------------
void ShowUsage( const char* programName )
{
    printf( "\n" );
    printf( "Usage:\n" );
    printf( "%s [Options]\n", programName );
    printf( "\t-h\t\t\tShow this message\n" );
    printf( "\t-world=WORLD_FILE\tLoad world from world file\n" );
    printf( "\n" );
}