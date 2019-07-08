#include "MDGame_PCH.h"
#include "MDGame/Test/GraphicComponent.h"
#include "Game/Serialization/MemberHandler.h"

///////////////////////////////////////////////////////////////////////////
IMPLEMENT_ENTITY_COMPONENT(CGraphicComponent)

BEGIN_DEFINE_DB_OBJECT(CGraphicComponent)
	SERIALIZE_MEMBER("MainTexture", m_mainTexture, BasicTypeHandler)
	SERIALIZE_MEMBER("TexturePivot", m_texturePivot, BasicTypeHandler)
	SERIALIZE_MEMBER("pos", m_position, BasicTypeHandler)
END_DEFINE_DB_OBJECT()

CGraphicComponent::CGraphicComponent()
	: m_mainTexture("")
	, m_texturePivot(0.0f, 0.0f)
	, m_position(0.0f,0.0f,0.0f)
{
    CLevel::TWorldExporterCallback callback( this, &CGraphicComponent::ExportResources );
    CLevel::Get()->RegisterWorldExporter( callback );
}

///////////////////////////////////////////////////////////////////////////
CGraphicComponent::~CGraphicComponent()
{
    CLevel::TWorldExporterCallback callback( this, &CGraphicComponent::ExportResources );
    CLevel::Get()->UnregisterWorldExporter( callback );
}

///////////////////////////////////////////////////////////////////////////
void CGraphicComponent::ExportResources( CResourceContainer& resourceContainer )
{
    resourceContainer.AddImageResource( m_mainTexture.m_filename );
}

///////////////////////////////////////////////////////////////////////////
IMPLEMENT_ENTITY_COMPONENT(CPhysicsComponent)

BEGIN_DEFINE_DB_OBJECT( CPhysicsComponent )
    SERIALIZE_MEMBER( "m2", m_memberTest, BasicTypeHandler )
END_DEFINE_DB_OBJECT()

CPhysicsComponent::CPhysicsComponent()
:m_memberTest( false )
{
}

///////////////////////////////////////////////////////////////////////////
IMPLEMENT_ENTITY_COMPONENT(CSoundComponent)

    BEGIN_DEFINE_DB_OBJECT( CSoundComponent )
    SERIALIZE_MEMBER( "m3", m_memberTest, BasicTypeHandler )
END_DEFINE_DB_OBJECT()

CSoundComponent::CSoundComponent()
:m_memberTest( 0.f )
{
}
