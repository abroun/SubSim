//------------------------------------------------------------------------------
// A simple simulator for the UWE group project
//------------------------------------------------------------------------------
#include <stdio.h>

#include "Simulator/Simulator.h"

//------------------------------------------------------------------------------
int main()
{   
    Simulator sim;
    
    if ( !sim.Init( "../data/SauceWorld.yaml" ) )
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
