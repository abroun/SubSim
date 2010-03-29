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

#include "Common/MathUtils.h"
#include "Entities/Sub.h"
#include "Entities/CoordinateSystemAxes.h"
#include "Entities/Gate.h"
#include "Entities/Buoy.h"
#include "Entities/Pool.h"
#include "Entities/CircularPool.h"
#include "Entities/FloorTarget.h"
#include "Entities/Pipe.h"
#include "Entities/SurveyWall.h"
#include "Entities/HarbourFloor.h"

//------------------------------------------------------------------------------
// Helper Routine Prototypes
//------------------------------------------------------------------------------
static Sub* XEP_BuildSub( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager, irr::video::IVideoDriver* pVideoDriver );
static Buoy* XEP_BuildBuoy( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager, btDiscreteDynamicsWorld* pPhysicsWorld );
static CoordinateSystemAxes* XEP_BuildCoordinateSystemAxes( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager );
static FloorTarget* XEP_BuildFloorTarget( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager );
static Gate* XEP_BuildGate( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager );
static Pool* XEP_BuildPool( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager );
static CircularPool* XEP_BuildCircularPool( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager );
static HarbourFloor* XEP_BuildHarbourFloor( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager );
static Pipe* XEP_BuildPipe( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager );
static SurveyWall* XEP_BuildSurveyWall( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager );

static void XEP_ParseAndSetRotation( xercesc::DOMNode* pNode, Entity* pEntity, bool bPrintErrors = false, bool bOptional = true );
static bool XEP_GetPosVectorElement( xercesc::DOMNode* pNode, Vector* pPosOut, bool bPrintErrors = false );
static bool XEP_GetVectorElement( xercesc::DOMNode* pNode, XMLCh* pTag, Vector* pVectorOut, bool bPrintErrors = false, bool bOptional = false );
static bool XEP_GetFloatElement( xercesc::DOMNode* pNode, XMLCh* pTag, F32* pFloatOut, bool bPrintErrors = false, bool bOptional = false );


//------------------------------------------------------------------------------
bool XmlEntityParser::BuildEntitiesFromXMLWorldFile( const char* worldFilename,
                                                     irr::scene::ISceneManager* pSceneManager, 
                                                     irr::video::IVideoDriver* pVideoDriver,
                                                     btDiscreteDynamicsWorld* pPhysicsWorld,
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
    XMLCh* pTypeAttributeTag = xercesc::XMLString::transcode( "type" );

    try 
    {
        pParser->parse( worldFilename );
        
        xercesc::DOMDocument* pDoc = pParser->getDocument();
        xercesc::DOMNodeList* pNodeList = pDoc->getElementsByTagName( pEntityTag );
        
        int numEntities = pNodeList->getLength();
        
        for ( int entityIdx = 0; entityIdx < numEntities; entityIdx++ )
        {
            xercesc::DOMNode* pEntityNode = pNodeList->item( entityIdx );
            
            // Determine the type of the entity
            Entity::eType entityType = Entity::eT_Invalid;
            
            xercesc::DOMNamedNodeMap* pAttributes = pEntityNode->getAttributes();
            if ( NULL != pAttributes )
            {
                xercesc::DOMNode* pTypeAttribute = pAttributes->getNamedItem( pTypeAttributeTag );
                if ( NULL != pTypeAttribute )
                {
                    char* pTypeString = xercesc::XMLString::transcode( pTypeAttribute->getTextContent() );
                    entityType = Entity::GetTypeFromString( pTypeString );
                    xercesc::XMLString::release( &pTypeString );
                }
            }
            
            // Create the entity
            Entity* pNewEntity = NULL;
            switch ( entityType )
            {
                case Entity::eT_Sub:
                {
                    pNewEntity = XEP_BuildSub( pEntityNode, pSceneManager, pVideoDriver );
                    break;
                }
                case Entity::eT_Buoy:
                {
                    pNewEntity = XEP_BuildBuoy( pEntityNode, pSceneManager, pPhysicsWorld );
                    break;
                }
                case Entity::eT_CoordinateSystemAxes:
                {
                    pNewEntity = XEP_BuildCoordinateSystemAxes( pEntityNode, pSceneManager );
                    break;
                }
                case Entity::eT_FloorTarget:
                {
                    pNewEntity = XEP_BuildFloorTarget( pEntityNode, pSceneManager );
                    break;
                }
                case Entity::eT_Gate:
                {
                    pNewEntity = XEP_BuildGate( pEntityNode, pSceneManager );
                    break;
                }
                case Entity::eT_Pool:
                {
                    pNewEntity = XEP_BuildPool( pEntityNode, pSceneManager );
                    break;
                }
                case Entity::eT_CircularPool:
                {
                    pNewEntity = XEP_BuildCircularPool( pEntityNode, pSceneManager );
                    break;
                }
                case Entity::eT_HarbourFloor:
                {
                    pNewEntity = XEP_BuildHarbourFloor( pEntityNode, pSceneManager );
                    break;
                }
                case Entity::eT_Pipe:
                {
                    pNewEntity = XEP_BuildPipe( pEntityNode, pSceneManager );
                    break;
                }
                case Entity::eT_SurveyWall:
                {
                    pNewEntity = XEP_BuildSurveyWall( pEntityNode, pSceneManager );
                    break;
                }
                default:
                {
                    fprintf( stderr, "Warning: Unable to identify type of entity %i\n", entityIdx );
                }
            }
                
            if ( NULL == pNewEntity )
            {
                fprintf( stderr, "Warning: Unable to build entity %i\n", entityIdx );
            }
            else
            {
                //printf( "Built a %s\n", Entity::ConvertTypeToString( pNewEntity->GetType() ) );
                pEntityListOut->push_back( pNewEntity );
            }
        }
                
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
        
    xercesc::XMLString::release( &pTypeAttributeTag );
    xercesc::XMLString::release( &pEntityTag );
    delete pParser;
    delete pErrHandler;
    
    xercesc::XMLPlatformUtils::Terminate();
    
    return bSuccessful;
}

//------------------------------------------------------------------------------
Sub* XEP_BuildSub( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager, irr::video::IVideoDriver* pVideoDriver )
{
    const bool PRINT_ERRORS = true;
    Sub* pSub = NULL;
    
    XMLCh* pYawTag = xercesc::XMLString::transcode( "yaw" );
    
    Vector pos;
    float yaw;
    
    if ( XEP_GetPosVectorElement( pEntityNode, &pos, PRINT_ERRORS )
         && XEP_GetFloatElement( pEntityNode, pYawTag, &yaw, PRINT_ERRORS ) )
    {
        pSub = new Sub();
        if ( !pSub->Init( pSceneManager, pVideoDriver ) )
        {
            fprintf( stderr, "Error: Unable to initialise sub\n" );
            delete pSub;
            pSub = NULL;
        }
        else
        {
            pSub->SetYaw( MathUtils::DegToRad( yaw ) );
            pSub->SetPosition( pos );
        }
    }
    
    xercesc::XMLString::release( &pYawTag );
    
    return pSub;
}

//------------------------------------------------------------------------------
Buoy* XEP_BuildBuoy( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager, btDiscreteDynamicsWorld* pPhysicsWorld )
{
    const bool PRINT_ERRORS = true;
    Buoy* pBuoy = NULL;
    
    Vector pos;
    
    if ( XEP_GetPosVectorElement( pEntityNode, &pos, PRINT_ERRORS ) )
    {
        const bool OPTIONAL = true;
        F32 radius = Buoy::DEFAULT_RADIUS;
        
        XMLCh* pRadiusTag = xercesc::XMLString::transcode( "radius" );
        XEP_GetFloatElement( pEntityNode, pRadiusTag, &radius, PRINT_ERRORS, OPTIONAL );
        xercesc::XMLString::release( &pRadiusTag );
        
        pBuoy = new Buoy();
        if ( !pBuoy->Init( pSceneManager, pPhysicsWorld, radius ) )
        {
            fprintf( stderr, "Error: Unable to initialise buoy\n" );
            delete pBuoy;
            pBuoy = NULL;
        }
        else
        {
            pBuoy->SetPosition( pos );
        }
    }
    
    return pBuoy;
}

//------------------------------------------------------------------------------
CoordinateSystemAxes* XEP_BuildCoordinateSystemAxes( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager )
{
    const bool PRINT_ERRORS = true;
    CoordinateSystemAxes* pCoordinateSystemAxes = NULL;
    
    Vector pos;
    
    if ( XEP_GetPosVectorElement( pEntityNode, &pos, PRINT_ERRORS ) )
    {
        pCoordinateSystemAxes = new CoordinateSystemAxes();
        if ( !pCoordinateSystemAxes->Init( pSceneManager ) )
        {
            fprintf( stderr, "Error: Unable to initialise coordinate system axes\n" );
            delete pCoordinateSystemAxes;
            pCoordinateSystemAxes = NULL;
        }
        else
        {
            pCoordinateSystemAxes->SetPosition( pos );
        }
    }
    
    return pCoordinateSystemAxes;
}

//------------------------------------------------------------------------------
FloorTarget* XEP_BuildFloorTarget( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager )
{
    const bool PRINT_ERRORS = true;
    FloorTarget* pFloorTarget = NULL;

    Vector pos;
    
    if ( XEP_GetPosVectorElement( pEntityNode, &pos, PRINT_ERRORS ) )
    {
        pFloorTarget = new FloorTarget();
        if ( !pFloorTarget->Init( pSceneManager ) )
        {
            fprintf( stderr, "Error: Unable to initialise floor target\n" );
            delete pFloorTarget;
            pFloorTarget = NULL;
        }
        else
        {
            pFloorTarget->SetPosition( pos );
        }
    }
    
    return pFloorTarget;
}

//------------------------------------------------------------------------------
Gate* XEP_BuildGate( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager )
{
    const bool PRINT_ERRORS = true;
    Gate* pGate = NULL;
    
    Vector pos;
    
    if ( XEP_GetPosVectorElement( pEntityNode, &pos, PRINT_ERRORS ) )
    {
        const bool OPTIONAL = true;
        F32 width = Gate::DEFAULT_WIDTH;
        F32 height = Gate::DEFAULT_HEIGHT;
        
        XMLCh* pWidthTag = xercesc::XMLString::transcode( "width" );
        XEP_GetFloatElement( pEntityNode, pWidthTag, &width, PRINT_ERRORS, OPTIONAL );
        xercesc::XMLString::release( &pWidthTag );
        XMLCh* pHeightTag = xercesc::XMLString::transcode( "height" );
        XEP_GetFloatElement( pEntityNode, pHeightTag, &height, PRINT_ERRORS, OPTIONAL );
        xercesc::XMLString::release( &pHeightTag );
        
        pGate = new Gate();
        if ( !pGate->Init( pSceneManager, width, height ) )
        {
            fprintf( stderr, "Error: Unable to initialise gate\n" );
            delete pGate;
            pGate = NULL;
        }
        else
        {
            pGate->SetPosition( pos );
            XEP_ParseAndSetRotation( pEntityNode, pGate, PRINT_ERRORS );
        }
    }    
    
    return pGate;
}

//------------------------------------------------------------------------------
Pool* XEP_BuildPool( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager )
{
    const bool PRINT_ERRORS = true;
    Pool* pPool = NULL;
    
    Vector pos;
    
    if ( XEP_GetPosVectorElement( pEntityNode, &pos, PRINT_ERRORS ) )
    {
        pPool = new Pool();
        if ( !pPool->Init( pSceneManager ) )
        {
            fprintf( stderr, "Error: Unable to initialise pool\n" );
            delete pPool;
            pPool = NULL;
        }
        else
        {
            pPool->SetPosition( pos );
        }
    }
    
    return pPool;
}

//------------------------------------------------------------------------------
CircularPool* XEP_BuildCircularPool( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager )
{
    const bool PRINT_ERRORS = true;
    CircularPool* pCircularPool = NULL;
    
    Vector pos;
    
    if ( XEP_GetPosVectorElement( pEntityNode, &pos, PRINT_ERRORS ) )
    {
        const bool OPTIONAL = true;
        F32 radius = CircularPool::DEFAULT_RADIUS;
        
        XMLCh* pRadiusTag = xercesc::XMLString::transcode( "radius" );
        XEP_GetFloatElement( pEntityNode, pRadiusTag, &radius, PRINT_ERRORS, OPTIONAL );
        xercesc::XMLString::release( &pRadiusTag );
        
        pCircularPool = new CircularPool();
        if ( !pCircularPool->Init( pSceneManager, radius ) )
        {
            fprintf( stderr, "Error: Unable to initialise circular pool\n" );
            delete pCircularPool;
            pCircularPool = NULL;
        }
        else
        {
            pCircularPool->SetPosition( pos );
        }
    }
    
    return pCircularPool;
}

//------------------------------------------------------------------------------
HarbourFloor* XEP_BuildHarbourFloor( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager )
{
    const bool PRINT_ERRORS = true;
    HarbourFloor* pHarbourFloor = NULL;
    
    Vector pos;
    
    if ( XEP_GetPosVectorElement( pEntityNode, &pos, PRINT_ERRORS ) )
    {
        pHarbourFloor = new HarbourFloor();
        if ( !pHarbourFloor->Init( pSceneManager ) )
        {
            fprintf( stderr, "Error: Unable to initialise harbour floor\n" );
            delete pHarbourFloor;
            pHarbourFloor = NULL;
        }
        else
        {
            pHarbourFloor->SetPosition( pos );
        }
    }
    
    return pHarbourFloor;
}

//------------------------------------------------------------------------------
Pipe* XEP_BuildPipe( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager )
{
    const bool PRINT_ERRORS = true;
    Pipe* pPipe = NULL;
    
    Vector pos;
    
    if ( XEP_GetPosVectorElement( pEntityNode, &pos, PRINT_ERRORS ) )
    {
        pPipe = new Pipe();
        if ( !pPipe->Init( pSceneManager ) )
        {
            fprintf( stderr, "Error: Unable to initialise pipe\n" );
            delete pPipe;
            pPipe = NULL;
        }
        else
        {
            pPipe->SetPosition( pos );
        }
    }
    
    return pPipe;
}

//------------------------------------------------------------------------------
SurveyWall* XEP_BuildSurveyWall( xercesc::DOMNode* pEntityNode, irr::scene::ISceneManager* pSceneManager )
{
    const bool PRINT_ERRORS = true;
    SurveyWall* pSurveyWall = NULL;
    
    Vector pos;
    
    if ( XEP_GetPosVectorElement( pEntityNode, &pos, PRINT_ERRORS ) )
    {
        pSurveyWall = new SurveyWall();
        if ( !pSurveyWall->Init( pSceneManager ) )
        {
            fprintf( stderr, "Error: Unable to initialise survey wall\n" );
            delete pSurveyWall;
            pSurveyWall = NULL;
        }
        else
        {
            pSurveyWall->SetPosition( pos );
            XEP_ParseAndSetRotation( pEntityNode, pSurveyWall, PRINT_ERRORS );
        }
    }
    
    return pSurveyWall;
}

//------------------------------------------------------------------------------
void XEP_ParseAndSetRotation( xercesc::DOMNode* pNode, Entity* pEntity, bool bPrintErrors, bool bOptional )
{
    bool bRotationFound = false;
    Vector rotationDegrees;
    
    XMLCh* pRotationTag = xercesc::XMLString::transcode( "rotation" );
    bRotationFound = XEP_GetVectorElement( pNode, pRotationTag, &rotationDegrees, bPrintErrors, bOptional );
    xercesc::XMLString::release( &pRotationTag );
    
    if ( bRotationFound )
    {
        pEntity->SetRotation( Vector( 
            MathUtils::DegToRad( rotationDegrees.mX ),
            MathUtils::DegToRad( rotationDegrees.mY ),
            MathUtils::DegToRad( rotationDegrees.mZ ) ) );
    }
    else if ( !bRotationFound && !bOptional && bPrintErrors )
    {
        fprintf( stderr, "Error: Unable to parse rotation element\n" );
    }
}

//------------------------------------------------------------------------------
bool XEP_GetPosVectorElement( xercesc::DOMNode* pNode, Vector* pPosOut, bool bPrintErrors )
{
    bool bFound = false;
    
    XMLCh* pPosTag = xercesc::XMLString::transcode( "pos" );
    bFound = XEP_GetVectorElement( pNode, pPosTag, pPosOut, bPrintErrors );
    xercesc::XMLString::release( &pPosTag );
    
    if ( !bFound && bPrintErrors )
    {
        fprintf( stderr, "Error: Unable to parse pos element\n" );
    }
    
    return bFound;
}

//------------------------------------------------------------------------------
bool XEP_GetVectorElement( xercesc::DOMNode* pNode, XMLCh* pTag, Vector* pVectorOut, bool bPrintErrors, bool bOptional )
{
    bool bFound = false;
    
    XMLCh* pXTag = xercesc::XMLString::transcode( "x" );
    XMLCh* pYTag = xercesc::XMLString::transcode( "y" );
    XMLCh* pZTag = xercesc::XMLString::transcode( "z" );
    
    xercesc::DOMNodeList* pChildNodeList = pNode->getChildNodes();
    int numChildNodes = pChildNodeList->getLength();
    for ( int childNodeIdx = 0; childNodeIdx < numChildNodes; childNodeIdx++ )
    {
        xercesc::DOMNode* pChildNode = pChildNodeList->item( childNodeIdx );
        if ( xercesc::XMLString::compareIString( pChildNode->getNodeName(), pTag ) == 0 )
        {
            // Found the vector node, now try to parse it
            
            F32 x, y, z;
            if ( XEP_GetFloatElement( pChildNode, pXTag, &x, bPrintErrors )
                && XEP_GetFloatElement( pChildNode, pYTag, &y, bPrintErrors )
                && XEP_GetFloatElement( pChildNode, pZTag, &z, bPrintErrors ) )
            {
                pVectorOut->Set( x, y, z );
                bFound = true;
            }
    
            break;
        }
    }
    xercesc::XMLString::release( &pZTag );
    xercesc::XMLString::release( &pYTag );
    xercesc::XMLString::release( &pXTag );
    
    if ( !bFound && !bOptional && bPrintErrors )
    {
        char* pNarrowTag = xercesc::XMLString::transcode( pTag );
        fprintf( stderr, "Error: Unable to parse vector called %s\n", pNarrowTag );
        xercesc::XMLString::release( &pNarrowTag );
    }
    
    return bFound;
}

//------------------------------------------------------------------------------
bool XEP_GetFloatElement( xercesc::DOMNode* pNode, XMLCh* pTag, F32* pFloatOut, bool bPrintErrors, bool bOptional )
{
    bool bFound = false;
    
    xercesc::DOMNodeList* pChildNodeList = pNode->getChildNodes();
    int numChildNodes = pChildNodeList->getLength();
    for ( int childNodeIdx = 0; childNodeIdx < numChildNodes; childNodeIdx++ )
    {
        xercesc::DOMNode* pChildNode = pChildNodeList->item( childNodeIdx );
        if ( xercesc::XMLString::compareIString( pChildNode->getNodeName(), pTag ) == 0 )
        {
            // Node found
            char* pDataString = xercesc::XMLString::transcode( pChildNode->getTextContent() );
            *pFloatOut = (float)atof( pDataString );
            xercesc::XMLString::release( &pDataString );
            
            bFound = true;
            break;
        }
    }
    
    if ( !bFound && !bOptional && bPrintErrors )
    {
        char* pNarrowTag = xercesc::XMLString::transcode( pTag );
        fprintf( stderr, "Error: Unable to parse float called %s\n", pNarrowTag );
        xercesc::XMLString::release( &pNarrowTag );
    }
    
    return bFound;
}
