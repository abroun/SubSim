//------------------------------------------------------------------------------
// File: XmlEntityParser.h
// Desc: Parses an XML file to find entities and their properties and then
// creates them.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef XML_ENTITY_PARSER_H
#define XML_ENTITY_PARSER_H

//------------------------------------------------------------------------------
#include <vector>

#include "Common.h"
#include "Entity.h"

//------------------------------------------------------------------------------
class btDiscreteDynamicsWorld;

//------------------------------------------------------------------------------
class XmlEntityParser
{
    public: static bool BuildEntitiesFromXMLWorldFile( 
        const char* worldFilename, 
        irr::scene::ISceneManager* pSceneManager, 
        irr::video::IVideoDriver* pVideoDriver,
        btDiscreteDynamicsWorld* pPhysicsWorld,
        std::vector<Entity*>* pEntityListOut );
};

#endif // XML_ENTITY_PARSER_H