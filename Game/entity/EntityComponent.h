#pragma once

#include "Game/DB/DBObject.h"

///////////////////////////////////////////////////////////////////////////
//IEntityComponent
///////////////////////////////////////////////////////////////////////////
class GAME_DLL IEntityComponent : public CDbObject
{
    DECLARE_DB_OBJECT( IEntityComponent, CDbObject );

public:
    IEntityComponent(){}
    virtual ~IEntityComponent(){}
};

///////////////////////////////////////////////////////////////////////////
#define DECLARE_ENTITY_COMPONENT_CREATOR(className)          \
    public:                                                                                 \
    static IEntityComponent* CreateComponent(const CStringID&) {return CreateComponent();}  \
    static className* CreateComponent()                                                     \
    {                                                                                       \
        className* newComponent = new className();                                          \
        newComponent->SetName( STRINGIZE( className ) );                                    \
        newComponent->SetKey( CStringID( STRINGIZE( className ) ) );                        \
        return newComponent;                                                                \
    }                                                                                       \
    static void RegisterEntityComponent();                                                  \
    static void UnregisterEntityComponent();

///////////////////////////////////////////////////////////////////////////
#define DECLARE_ENTITY_COMPONENT(className, parentClassName)    \
    public:                                                     \
    DECLARE_DB_OBJECT(className,parentClassName)                \
    DECLARE_ENTITY_COMPONENT_CREATOR(className)

///////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_ENTITY_COMPONENT(className)			\
    void className::RegisterEntityComponent()			\
    {													\
        className::RegisterProperties();				\
    }													\
    void className::UnregisterEntityComponent()			\
    {													\
    }

#define REGISTER_ENTITY_COMPONENT(className)			\
    className::RegisterEntityComponent();
