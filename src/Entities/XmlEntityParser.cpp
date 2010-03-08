//------------------------------------------------------------------------------
// File: XmlEntityParser.cpp
// Desc: Parses an XML file to find entities and their properties and then
// creates them.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "XmlEntityParser.h"

#include <assert.h>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

//------------------------------------------------------------------------------
bool XmlEntityParser::BuildEntitiesFromXMLWorldFile( const char* worldFilename,
                                                     std::vector<Entity*>* pEntityListOut )
{
    assert( NULL != pEntityListOut && "No entity list provided" );
    
    bool bSuccessful = true;
    
    xercesc::XMLPlatformUtils::Initialize();
    
    xercesc::XercesDOMParser* pParser = new xercesc::XercesDOMParser();
    pParser->setValidationScheme( xercesc::XercesDOMParser::Val_Always );
    pParser->setDoNamespaces( true );    // optional

    xercesc::ErrorHandler* pErrHandler = 
        (xercesc::ErrorHandler*)new xercesc::HandlerBase();
    pParser->setErrorHandler( pErrHandler );

    XMLCh* pEntityTag = xercesc::XMLString::transcode( "entity" );

    try 
    {
        pParser->parse( worldFilename );
        
        xercesc::DOMDocument* pDoc = pParser->getDocument();
        xercesc::DOMNodeList* pNodeList = pDoc->getElementsByTagName( pEntityTag );
         
        
        printf( "Opened file...\n" );
        printf( "Found %i entities\n", pNodeList->getLength() );
        
        
        
    }
    catch ( const xercesc::XMLException& toCatch ) 
    {
        char* message = xercesc::XMLString::transcode( toCatch.getMessage() );
        fprintf( stderr, "Error: XML Exception message is: %s\n", message );
        xercesc::XMLString::release( &message );
        bSuccessful = false;
    }
    catch ( const xercesc::DOMException& toCatch ) 
    {
        char* message = xercesc::XMLString::transcode( toCatch.msg );
        fprintf( stderr, "Error: DOM Exception message is: %s\n", message );
        xercesc::XMLString::release( &message );
        bSuccessful = false;
    }
    catch (...) 
    {
        fprintf( stderr, "Error: Unexpected Exception\n" );
        bSuccessful = false;
    }
        
    delete[] pEntityTag;
    delete pParser;
    delete pErrHandler;
    
    xercesc::XMLPlatformUtils::Terminate();
    
    return bSuccessful;
}

