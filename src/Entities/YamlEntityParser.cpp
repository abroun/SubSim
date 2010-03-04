//------------------------------------------------------------------------------
// File: YamlEntityParser.cpp
// Desc: Parses a YAML file to find entities and their properties
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include <yaml.h>
#include <assert.h>
#include "Entity.h"
#include "YamlEntityParser.h"

//------------------------------------------------------------------------------
const char* YamlEventTypeToString( yaml_event_type_t eventType );
const char* YamlTagToString( yaml_char_t* pTag );

//------------------------------------------------------------------------------
YamlEntityList* YamlEntityParser::ParseYamlFile( const char* filename )
{
    YamlEntityList* pEntityList = new YamlEntityList();
    bool bParsingSuccessful = true;

    yaml_parser_t parser;
    yaml_event_t event;

    // Create the Parser object
    yaml_parser_initialize( &parser );

    // Set a file input
    FILE* pYamlFile = fopen( filename, "rb" );
    if ( NULL == pYamlFile )
    {
        bParsingSuccessful = false;
        fprintf( stderr, "Error: Unable to open file - %s\n", filename );
    }
    else
    {
        yaml_parser_set_input_file( &parser, pYamlFile );
        
        /* Read the event sequence. */
        bool bParsingFinished = false;
        eParserState parserState = ePS_WaitingForEntity;
        int mappingDepth = 0;
        while ( !bParsingFinished ) 
        {
            // Get the next event.
            if ( !yaml_parser_parse( &parser, &event ) )
            {
                fprintf( stderr, "Error: YAML Parsing error - %s\n", parser.problem );
                bParsingSuccessful = false;
                break;
            }
            
            if ( event.type == YAML_MAPPING_START_EVENT )
            {
                printf( "Tag = %s\n", event.data.mapping_start.tag );
            }

            switch ( parserState )
            {
                case ePS_WaitingForEntity:
                {
                    if ( event.type == YAML_MAPPING_START_EVENT )
                    {
                        const char* pTypeString = YamlTagToString( event.data.mapping_start.tag );
                        
                        Entity::eType entityType = Entity::GetTypeFromString( pTypeString );
                        if ( Entity::eT_Invalid == entityType )
                        {
                            if ( NULL != pTypeString
                                && '\0' != pTypeString[ 0 ] )
                            {
                                fprintf( stderr, "Warning: Found unrecognised entity type - %s\n",
                                    pTypeString );
                            }
                            mappingDepth = 1;
                            parserState = ePS_SkippingPastNonEntity;
                        }
                        else
                        {
                            printf( "---- Found %s entity\n", Entity::ConvertTypeToString( entityType ) );
                            parserState = ePS_ParsingEntity;
                        }
                    }
                    break;
                }
                case ePS_SkippingPastNonEntity:
                {
                    if ( event.type == YAML_MAPPING_START_EVENT )
                    {
                        mappingDepth++;
                    }
                    else if ( event.type == YAML_MAPPING_END_EVENT )
                    {
                        mappingDepth--;
                        if ( mappingDepth <= 0 )
                        {
                            parserState = ePS_WaitingForEntity;
                        }
                    }
                    break;
                }
                case ePS_ParsingEntity:
                {
                    if ( event.type == YAML_MAPPING_END_EVENT )
                    {
                        parserState = ePS_WaitingForEntity;
                    }
                    break;
                }
                default:
                {
                   assert( false && "Unhandled parser state encountered" ); 
                }
            }
            /*
                ...
                Process the event.
                ...
            */
            printf( "Found - %s\n", YamlEventTypeToString( event.type ) );
            

            // Are we finished?
            bParsingFinished = (event.type == YAML_STREAM_END_EVENT);

            // The application is responsible for destroying the event object.
            yaml_event_delete( &event );

        }

    }

    
    
Finished:
    // Destroy the Parser object.
    yaml_parser_delete( &parser );
    
    if ( NULL != pYamlFile )
    {
        fclose( pYamlFile );
    }

    // Only return the entity list if parsing has been successful
    if ( !bParsingSuccessful )
    {
        if ( NULL != pEntityList )
        {
            delete pEntityList;
            pEntityList = NULL;
        }
    }
    return pEntityList;
}

//------------------------------------------------------------------------------
const char* YamlEventTypeToString( yaml_event_type_t eventType )
{
    const char* pResult;
    
    switch ( eventType )
    {
        case YAML_NO_EVENT:
        {
            pResult = "YAML_NO_EVENT";
            break;
        }
        case YAML_STREAM_START_EVENT:
        {
            pResult = "YAML_STREAM_START_EVENT";
            break;
        }
        case YAML_STREAM_END_EVENT:
        {
            pResult = "YAML_STREAM_END_EVENT";
            break;
        }
        case YAML_DOCUMENT_START_EVENT:
        {
            pResult = "YAML_DOCUMENT_START_EVENT";
            break;
        }
        case YAML_DOCUMENT_END_EVENT:
        {
            pResult = "YAML_DOCUMENT_END_EVENT";
            break;
        }
        case YAML_ALIAS_EVENT:
        {
            pResult = "YAML_ALIAS_EVENT";
            break;
        }
        case YAML_SCALAR_EVENT:
        {
            pResult = "YAML_SCALAR_EVENT";
            break;
        }
        case YAML_SEQUENCE_START_EVENT:
        {
            pResult = "YAML_SEQUENCE_START_EVENT";
            break;
        }
        case YAML_SEQUENCE_END_EVENT:
        {
            pResult = "YAML_SEQUENCE_END_EVENT";
            break;
        }
        case YAML_MAPPING_START_EVENT:
        {
            pResult = "YAML_MAPPING_START_EVENT";
            break;
        }
        case YAML_MAPPING_END_EVENT:
        {
            pResult = "YAML_MAPPING_END_EVENT";
            break;
        }
        default:
        {
            pResult = "UNKNOWN";
        }
    }
    
    return pResult;
}

//------------------------------------------------------------------------------
const char* YamlTagToString( yaml_char_t* pTag )
{
    const char* pResult = NULL;
    if ( pTag != NULL )
    {
        if ( '!' == pTag[ 0 ] )
        {
            // Skip past '!' if present
            pResult = (const char*)&pTag[ 1 ];
        }
        else
        {
            pResult = (const char*)pTag;
        }
    }
    
    return pResult;
}