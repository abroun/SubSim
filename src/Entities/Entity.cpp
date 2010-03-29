//------------------------------------------------------------------------------
// File: Entity.cpp
// Desc: A good old fashioned base entity class
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include "Entity.h"
#include "Common/MathUtils.h"
#include "Common/Utils.h"

//------------------------------------------------------------------------------
const char* Entity::TYPE_NAMES[ eT_NumTypes ] =
{
    "Sub",
    "Buoy",
    "CoordinateSystemAxes",
    "FloorTarget",
    "Gate",
    "Pool",
    "CircularPool",
    "HarbourFloor",
    "Pipe",
    "SurveyWall"
};

//------------------------------------------------------------------------------
Entity::eType Entity::GetTypeFromString( const char* pTypeString )
{
    eType result = eT_Invalid;
    
    if ( NULL != pTypeString )
    {
        for ( int typeIdx = 0; typeIdx < eT_NumTypes; typeIdx++ )
        {
            if ( Utils::stricmp( TYPE_NAMES[ typeIdx ], pTypeString ) == 0 )
            {
                // Type found
                result = (eType)typeIdx;
                break;
            }
        }
    }
    
    return result;
}

//------------------------------------------------------------------------------
const char* Entity::ConvertTypeToString( eType type )
{
    const char* pResult = NULL;
    if ( type >= 0 && type < eT_NumTypes )
    {
        pResult = TYPE_NAMES[ type ];
    }
    
    return pResult;
}

//------------------------------------------------------------------------------
Entity::Entity()
    : mbInitialised( false ),
    mpSceneManager( NULL ),
    mpTransformNode( NULL )
{
}

//------------------------------------------------------------------------------
Entity::~Entity()
{
    DeInit();
}

//------------------------------------------------------------------------------
bool Entity::Init( irr::scene::ISceneManager* pSceneManager )
{
    if ( !mbInitialised )
    {
        mpSceneManager = pSceneManager;
        mpSceneManager->grab();
        
        // Create a parent node to hold any SubSim transformations
        mpTransformNode = mpSceneManager->addDummyTransformationSceneNode();
        if ( NULL == mpTransformNode )
        {
            DeInit();
            return false;
        }
        
        mTranslation.Set( 0.0f, 0.0f, 0.0f );
        mRotation.Set( 0.0f, 0.0f, 0.0f );
        UpdateTransform();

        mbInitialised = true;
    }

    return true;
}

//------------------------------------------------------------------------------
void Entity::DeInit()
{
    if ( NULL != mpTransformNode )
    {
        mpTransformNode = NULL;
    }

    if ( NULL != mpSceneManager )
    {
        mpSceneManager->drop();
        mpSceneManager = NULL;
    }

    mbInitialised = false;
}

//------------------------------------------------------------------------------
void Entity::SetPosition( const Vector& pos )
{
    if ( mbInitialised )
    {
        mTranslation = pos;
        UpdateTransform();
    }
}

//------------------------------------------------------------------------------
const Vector& Entity::GetPosition() const
{
    return mTranslation;
}

//------------------------------------------------------------------------------
void Entity::SetRotation( const Vector& rotation )
{
    if ( mbInitialised )
    {
        mRotation = rotation;
        UpdateTransform();
    }
}

//------------------------------------------------------------------------------
const Vector& Entity::GetRotation() const
{
    return mRotation;
}

//------------------------------------------------------------------------------
void Entity::SetYaw( F32 yawAngle )
{
    if ( mbInitialised )
    {
        mRotation.mZ = yawAngle;
        UpdateTransform();
    }
}

//------------------------------------------------------------------------------
F32 Entity::GetYaw() const
{
    return mRotation.mZ;
}

//------------------------------------------------------------------------------
void Entity::UpdateTransform()
{
    irr::core::vector3df irrRotation = MathUtils::TransformRotation_SubToIrr( mRotation );
    irr::core::vector3df irrTranslation = MathUtils::TransformVector_SubToIrr( mTranslation );
    
    irr::core::matrix4& subTransform = mpTransformNode->getRelativeTransformationMatrix();
    subTransform.setRotationRadians( irrRotation );
    subTransform.setTranslation( irrTranslation );
}

//------------------------------------------------------------------------------
void Entity::AddChildNode( irr::scene::ISceneNode* pChildNode )
{
    if ( mbInitialised )
    {
        mpTransformNode->addChild( pChildNode );
    }
}

//------------------------------------------------------------------------------
void Entity::RemoveChildNode( irr::scene::ISceneNode* pChildNode )
{
    if ( mbInitialised )
    {
        mpTransformNode->removeChild( pChildNode );
    }
}

//------------------------------------------------------------------------------
void Entity::RemoveAllChildNodes()
{
    if ( mbInitialised )
    {
        mpTransformNode->removeAll();
    }
}
