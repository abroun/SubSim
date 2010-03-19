//------------------------------------------------------------------------------
// File: CommandLineParser.cpp
// Desc: A simple command line parser that reads in arguments of the form
//        -arg and -arg=value
//
// Author: Alan Broun
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "CommandLineParser.h"

#include <string.h>
#include "Utils.h"

//------------------------------------------------------------------------------
char CommandLineParser::mArgNames[ MAX_NUM_ARGS ][ MAX_ARG_LENGTH + 1 ];
char CommandLineParser::mArgValues[ MAX_NUM_ARGS ][ MAX_ARG_VALUE_LENGTH + 1 ];
int CommandLineParser::mNumArgs = 0;

//------------------------------------------------------------------------------
void CommandLineParser::ParseCommandLine( int argc, const char** argv )
{
    // Loop through the arguments, skipping past argument 0 which is
    // the name of the program
    for ( int i = 1; i < argc; i++ )
    {
        // An argument is identified by a leading '-'
        if ( argv[ i ][ 0 ] == '-' )
        {
            if ( mNumArgs < MAX_NUM_ARGS )
            {
                int argNameLength = 0;
                int argValueStartIdx = -1;
                int argValueLength = 0;
                bool bArgValueFound = false;
                int testCharIdx = 1;
                
                while ( argv[ i ][ testCharIdx ] != '\0' )
                {
                    if ( !bArgValueFound )
                    {
                        if ( argv[ i ][ testCharIdx ] == '=' )
                        {
                            argValueStartIdx = testCharIdx + 1;
                            bArgValueFound = true;
                        }
                        else
                        {
                            argNameLength++;
                        }
                    }
                    else
                    {
                        argValueLength++;
                    }
                    testCharIdx++;
                }
                
                if ( argNameLength > 0 )
                {
                    if ( argNameLength > MAX_ARG_LENGTH )
                    {
                        fprintf( stderr, "Warning: Argument name can not be longer than "
                            "MAX_ARG_LENGTH (%i). Truncating...\n", MAX_ARG_LENGTH );
                        argNameLength = MAX_ARG_LENGTH;
                    }
                    
                    strncpy( mArgNames[ mNumArgs ], &argv[ i ][ 1 ], argNameLength );
                    mArgNames[ mNumArgs ][ argNameLength ] = '\0';
                    
                    if ( bArgValueFound )
                    {
                        if ( argValueLength > MAX_ARG_VALUE_LENGTH )
                        {
                            fprintf( stderr, "Warning: Argument value can not be longer than "
                                "MAX_ARG_VALUE_LENGTH (%i). Truncating...\n", MAX_ARG_VALUE_LENGTH );
                            argValueLength = MAX_ARG_VALUE_LENGTH;
                        }
                        
                        strncpy( mArgValues[ mNumArgs ], &argv[ i ][ argValueStartIdx ], argValueLength );
                        mArgValues[ mNumArgs ][ argValueLength ] = '\0';
                    }
                    else
                    {
                        mArgValues[ mNumArgs ][ 0 ] = '\0';
                    }
                    
                    mNumArgs++;
                }
            }
            else
            {
                fprintf( stderr, "Warning: Not enough space for argument %i\n", i );
            }
        }
    }
}
    
//------------------------------------------------------------------------------
bool CommandLineParser::IsArgSet( const char* argName )
{
    bool bResult = false;
    
    for ( int argIdx = 0; argIdx < mNumArgs; argIdx++ )
    {
        if ( Utils::stricmp( mArgNames[ argIdx ], argName ) == 0 )
        {
            bResult = true;
            break;
        }
    }
    
    return bResult;
}

//------------------------------------------------------------------------------
const char* CommandLineParser::GetArgValue( const char* argName )
{
    const char* pResult = NULL;
    
    for ( int argIdx = 0; argIdx < mNumArgs; argIdx++ )
    {
        if ( Utils::stricmp( mArgNames[ argIdx ], argName ) == 0 )
        {
            if ( strlen( mArgValues[ argIdx ] ) > 0 )
            {
                pResult = mArgValues[ argIdx ];
            }
            break;
        }
    }
    
    return pResult;
}
