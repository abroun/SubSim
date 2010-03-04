//------------------------------------------------------------------------------
// File: YamlEntityParser.h
// Desc: Parses a YAML file to find entities and their properties
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef YAML_ENTITY_PARSER_H
#define YAML_ENTITY_PARSER_H

//------------------------------------------------------------------------------
#include "Common.h"
#include "Vector.h"
#include <vector>

//------------------------------------------------------------------------------
class YamlData
{
    enum eType
    {
        eT_Int,
        eT_Float,
        eT_Vector,
    };
    
    public: eType mType;
    public: char mName[ 64 ];
};

//------------------------------------------------------------------------------
class YamlDataInt : public YamlData
{
    public: int mData;
};

//------------------------------------------------------------------------------
class YamlDataFloat : public YamlData
{
    public: float mData;
};

//------------------------------------------------------------------------------
class YamlDataVector : public YamlData
{
    public: Vector mData;
};

//------------------------------------------------------------------------------
class YamlEntity
{
    public: YamlEntity();
    public: ~YamlEntity();
    
    public: std::vector<YamlData*> mData;
};

//------------------------------------------------------------------------------
class YamlEntityList
{
    public: YamlEntityList() {}
    public: ~YamlEntityList() {}
    
    public: int GetNumEntities() const;
    public: const YamlEntity* GetEntity( int entityIdx ) const;
    
    private: std::vector<YamlEntity*> mEntityList;
};

//------------------------------------------------------------------------------
class YamlEntityParser
{
    enum eParserState
    {
        ePS_WaitingForStartOfData,
        ePS_WaitingForEntity,
        ePS_SkippingPastNonEntity,
        ePS_ParsingEntity
    };
    
    public: static YamlEntityList* ParseYamlFile( const char* filename );
};

#endif // YAML_ENTITY_PARSER_H