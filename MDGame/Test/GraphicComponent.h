#pragma once
#include "Game\Entity\EntityComponent.h"
#include "MDGame/MDGameDefs.h"
#include "Game/Gameplay/Level.h"
#include "Game/Core/Math.h"
#include "Game\Gameplay\ResourceContainer.h"

///////////////////////////////////////////////////////////////////////////
//CGraphicComponent
///////////////////////////////////////////////////////////////////////////
class MDGAME_DLL CGraphicComponent : public IEntityComponent
{
public:
    DECLARE_ENTITY_COMPONENT( CGraphicComponent, IEntityComponent );
public:
    CGraphicComponent();
    virtual ~CGraphicComponent();

    const CDBFile& GetTextureFile() const{ return m_mainTexture; }
    vector2 GetPivot() const{ return m_texturePivot; }

private:
    void ExportResources( CResourceContainer& resourceContainer );

    CDBFile     m_mainTexture;
    vector2       m_texturePivot;
    vector3       m_position;
};

///////////////////////////////////////////////////////////////////////////
//CPhysicsComponent
///////////////////////////////////////////////////////////////////////////
class MDGAME_DLL CPhysicsComponent : public IEntityComponent
{
public:
    DECLARE_ENTITY_COMPONENT( CPhysicsComponent, IEntityComponent );
public:
    CPhysicsComponent();

    bool m_memberTest;
};

///////////////////////////////////////////////////////////////////////////
//CSoundComponent
///////////////////////////////////////////////////////////////////////////
class MDGAME_DLL CSoundComponent : public IEntityComponent
{
public:
    DECLARE_ENTITY_COMPONENT( CSoundComponent, IEntityComponent );
public:
    CSoundComponent();

    float m_memberTest;
};
