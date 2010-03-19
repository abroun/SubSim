//------------------------------------------------------------------------------
// File: CommandLineParser.h
// Desc: A simple command line parser that reads in arguments of the form
//        -arg and -arg=value
//
// Author: Alan Broun
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

//------------------------------------------------------------------------------
#include "Common.h"

//------------------------------------------------------------------------------
class CommandLineParser
{
    public: static void ParseCommandLine( S32 argc, const char** argv );
    
    public: static bool IsArgSet( const char* argName );
    public: static const char* GetArgValue( const char* argName );
    public: static S32 GetNumArgs() { return mNumArgs; }
    
    private: static const S32 MAX_NUM_ARGS = 32;
    private: static const S32 MAX_ARG_LENGTH = 64;
    private: static const S32 MAX_ARG_VALUE_LENGTH = 512;
    
    private: static char mArgNames[ MAX_NUM_ARGS ][ MAX_ARG_LENGTH + 1 ];
    private: static char mArgValues[ MAX_NUM_ARGS ][ MAX_ARG_VALUE_LENGTH + 1 ];
    private: static int mNumArgs;
};

#endif // COMMAND_LINE_PARSER_H
