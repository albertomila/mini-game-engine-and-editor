#include "Game_PCH.h"
#include "Game/Gameplay/Level.h"

#include "Game/Core/Math.h"
#include "Game/Core/Path.h"
#include "Game/Core/StringToType.h"
#include "Game/Core/TypeToString.h"
#include "Game/entity/Entity.h"
#include "Game/Entity/EntityComponent.h"
#include "Game/entity/EntityLibrary.h"
#include "Game/entity/EntityRegistry.h"
#include <algorithm>

///////////////////////////////////////////////////////////////////////////
CLevel::CLevel()
{
}

///////////////////////////////////////////////////////////////////////////
CLevel::~CLevel()
{
}

///////////////////////////////////////////////////////////////////////////
void CLevel::Load(std::string& levelFileName, std::string& levelName )
{
    CleanEntities();

    m_levelFileName = levelFileName;
    m_levelName = levelName;

    m_countEntities.clear();

	std::string filename;
	PathHelpers::GetLevelsPath(filename);
    filename.append( m_levelFileName );

    pugi::xml_document doc;
    if ( doc.load_file( filename.c_str() ))
    {
        pugi::xml_node xmlRootNode = doc.child( "level" );
        if (xmlRootNode)
        {
            pugi::xml_node xmlEntitiesNode = xmlRootNode.child( "Entities" );
            if (xmlEntitiesNode)
            {
                for (pugi::xml_node_iterator it = xmlEntitiesNode.begin(); it != xmlEntitiesNode.end(); ++it)
                {
                    const pugi::xml_node& xmlEntityNode = *it;

                    pugi::xml_attribute xmlAttributeKey = xmlEntityNode.attribute( "id" );
                    pugi::xml_attribute xmlAttributeType = xmlEntityNode.attribute( "type" );
                    pugi::xml_attribute xmlAttributeName = xmlEntityNode.attribute( "name" );
                    pugi::xml_attribute xmlAttributePos = xmlEntityNode.attribute( "pos" );
                    pugi::xml_attribute xmlAttributeParentID = xmlEntityNode.attribute( "parentID" );

                    std::string key( xmlAttributeKey.value() );
                    std::string typeString( xmlAttributeType.value() );
                    std::string name( xmlAttributeName.value() );

                    if( !xmlAttributeParentID.empty() )
                    {
                        std::string parentID( xmlAttributeParentID.value() );
                        if( !parentID.empty() )
                        {
                            CStringID::IDType parentKeyCrc;
                            StringUtils::StringToType( parentID.c_str() , parentKeyCrc );
                            if( parentKeyCrc != CStringID::InvalidID )
                            {
                                IncreaseNextEntityCountSufix( parentKeyCrc );
                            }
                        }
                    }

                    vector3 pos;
                    StringUtils::StringToType( xmlAttributePos.value(), pos );

                    CStringID::IDType keyCrc;
                    StringUtils::StringToType( key.c_str() , keyCrc );
                    CStringID entityKey( keyCrc, name.c_str() );

                    CStringID::IDType typeCrc;
                    StringUtils::StringToType( typeString.c_str() , typeCrc );
                    CStringID entityType( typeCrc );

                    CEntity* entity = CEntityRegistry::Get()->CreateNewEntity( entityType, CStringID( xmlAttributeName.value() ) );
                    entity->SetKey( entityKey );
                    entity->SetName( name );
                    entity->SetPos( pos );

                    CEntity::TComponentMap& components = entity->GetComponents();
                    FOR_EACH( CEntity::TComponentMap, it, components )
                    {
                        IEntityComponent* component = it->second;

                        pugi::xml_node componentXmlNode = xmlEntityNode.child( component->GetName() );

                        CDbObject* dbObject = component;

                        dbObject->GetDescriptor().Load( dbObject, &componentXmlNode );
                    }

                    m_entities.push_back( entity );
                }
            }

            LoadResources( xmlRootNode );
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void CLevel::Save()
{
    pugi::xml_document doc;
    pugi::xml_node rootNode = doc.append_child();
    rootNode.set_name("level");
    rootNode.append_attribute("name") = m_levelName.c_str();

    pugi::xml_node entitiesObjectNode = rootNode.append_child();
    entitiesObjectNode.set_name( "Entities" );

    FOR_EACH( std::vector<CEntity*>, it, m_entities)
    {
        CEntity* entity = *it;
        pugi::xml_node libraryObjectNode = entitiesObjectNode.append_child();
        libraryObjectNode.set_name( "Entity" );
        libraryObjectNode.append_attribute("name") = entity->GetName();
        libraryObjectNode.append_attribute("id")  = StringUtils::TypeToString( entity->GetKey().GetUniqueID() ).c_str();
        libraryObjectNode.append_attribute("type") = StringUtils::TypeToString( entity->GetType().GetUniqueID() ).c_str();
        libraryObjectNode.append_attribute("pos")  = StringUtils::TypeToString( entity->GetPos() ).c_str();

        if( entity->HasParent() )
        {
            libraryObjectNode.append_attribute("parentID")  = StringUtils::TypeToString( entity->GetParent().GetUniqueID() ).c_str();
        }

        const CEntity::TComponentMap& components = entity->GetComponents();
        FOR_EACH_CONST( CEntity::TComponentMap, it, components )
        {
            const IEntityComponent* component = it->second;

            pugi::xml_node componentXmlNode = libraryObjectNode.append_child( component->GetName() );

            const CDbObject* dbObjectConst = component;

            dbObjectConst->GetDescriptor().Save( dbObjectConst, &componentXmlNode );
        }
    }

    ExportResources( rootNode );

    std::string filename;
	PathHelpers::GetLevelsPath(filename);
    filename.append( m_levelFileName );
    doc.save_file( filename.c_str() );
}

///////////////////////////////////////////////////////////////////////////
void CLevel::AddEntity( CEntity* entity )
{
    FOR_EACH( std::vector<CEntity*>, it, m_entities)
    {
        if( (*it)->GetKey() == entity->GetKey() )
        {
            return;
        }
    }

    m_entities.push_back( entity );
}

///////////////////////////////////////////////////////////////////////////
void CLevel::RemoveEntity( CEntity* entity )
{
    FOR_EACH( std::vector<CEntity*>, it, m_entities)
    {
        if( (*it)->GetKey() == entity->GetKey() )
        {
            m_entities.erase( it );
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void CLevel::CleanEntities()
{
    m_entities.clear();
}

///////////////////////////////////////////////////////////////////////////
U32 CLevel::GetNextEntityCountSufix( CStringID entityID )
{
    TCountEntitiesMap::iterator it = m_countEntities.find( entityID.GetUniqueID() );
    if( it != m_countEntities.end() )
    {
        return ++(it->second);
    }
    else
    {
        m_countEntities[ entityID.GetUniqueID() ] = 0;

        return 0;
    }
}

///////////////////////////////////////////////////////////////////////////
void CLevel::IncreaseNextEntityCountSufix( CStringID::IDType entityID )
{
    TCountEntitiesMap::iterator it = m_countEntities.find( entityID );
    if( it != m_countEntities.end() )
    {
        ++(it->second);
    }
    else
    {
        m_countEntities[ entityID ] = 0;
    }
}

///////////////////////////////////////////////////////////////////////////
void CLevel::RegisterWorldExporter( const TWorldExporterCallback& callback )
{
    m_worldExporterCallbacks.push_back( callback );
}

///////////////////////////////////////////////////////////////////////////
void CLevel::UnregisterWorldExporter( const TWorldExporterCallback& callback )
{
    TWorldExporterVector::iterator it = std::find(m_worldExporterCallbacks.begin(), m_worldExporterCallbacks.end(), callback);
    if (it != m_worldExporterCallbacks.end())
    {
        m_worldExporterCallbacks.erase( it );
    }
}

///////////////////////////////////////////////////////////////////////////
void CLevel::ExportResources( pugi::xml_node& xmlRootNode )
{
    pugi::xml_node resourcesXmlNode = xmlRootNode.append_child();
    resourcesXmlNode.set_name( "Resources" );

    pugi::xml_node texturesXmlNode = resourcesXmlNode.append_child();
    texturesXmlNode.set_name( "Textures" );

    pugi::xml_node soundXmlNode = resourcesXmlNode.append_child();
    soundXmlNode.set_name( "Sounds" );

    m_resourceContainer.Clear();

    FOR_EACH( TWorldExporterVector, it, m_worldExporterCallbacks)
    {
        it->Call( m_resourceContainer );
    }
    
    const CResourceContainer::TRecourceList& resources = m_resourceContainer.GetResources(); 
    FOR_EACH_CONST( CResourceContainer::TRecourceList, it, resources)
    {
        IResource* resource = *it;
        if( CImageResource* imageResource = dynamic_cast<CImageResource*>( resource ) )
        {
            pugi::xml_node textureXmlNode = texturesXmlNode.append_child();
            textureXmlNode.set_name( "Texture" );
            textureXmlNode.append_attribute("path") = imageResource->GetResource().c_str();
        }
        else if( CSoundResource* imageResource = dynamic_cast<CSoundResource*>( resource ) )
        {
            pugi::xml_node soundXmlNode = soundXmlNode.append_child();
            soundXmlNode.set_name( "Texture" );
            soundXmlNode.append_attribute("path") = imageResource->GetResource().c_str();
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void CLevel::LoadResources( pugi::xml_node& xmlRootNode )
{
    m_resourceContainer.Clear();

    pugi::xml_node resourcesXmlNode = xmlRootNode.child( "Resources" );
    if (resourcesXmlNode)
    {
        pugi::xml_node texturesXmlNode = resourcesXmlNode.child( "Textures" );
        if (texturesXmlNode)
        {
            for (pugi::xml_node_iterator it = texturesXmlNode.begin(); it != texturesXmlNode.end(); ++it)
            {
                const pugi::xml_node& textureXmlNode = *it;

                pugi::xml_attribute xmlAttributeName = textureXmlNode.attribute( "path" );
                std::string path( xmlAttributeName.value() );

                m_resourceContainer.AddImageResource( path );
            }
        }

        pugi::xml_node soundsXmlNode = resourcesXmlNode.child( "Sounds" );
        if (soundsXmlNode)
        {
            for (pugi::xml_node_iterator it = soundsXmlNode.begin(); it != soundsXmlNode.end(); ++it)
            {
                const pugi::xml_node& soundXmlNode = *it;

                pugi::xml_attribute xmlAttributeName = soundXmlNode.attribute( "path" );
                std::string path( xmlAttributeName.value() );

                m_resourceContainer.AddImageResource( path );
            }
        }
    }

    m_resourceContainer.LoadResources();
}