#include "Game_PCH.h"
#include "Game/Entity/EntityLibrary.h"

#include "Game/Core/Algorithm.h"
#include "Game/Core/Path.h"
#include "Game/Core/StringToType.h"
#include "Game/Core/StringToType.h"
#include "Game/Core/TypeToString.h"
#include "Game/Entity/EntityComponent.h"
#include "Game/Entity/EntityRegistry.h"
#include "Game/Gameplay/Level.h"
#include "Game/Utils/FileHelpers.h"

CEntityLibrary::CEntityLibrary() {}
CEntityLibrary::~CEntityLibrary()
{
	DeleteMap(m_entityMap);
}

void CEntityLibrary::LoadAll()
{
    std::vector<std::string> listFiles;
    GetEntityLibraryFilePaths( listFiles);

    FOR_EACH_CONST( std::vector<std::string>, it, listFiles )
    {
		std::string filename;
		PathHelpers::GetEntitiesPath(filename);
        filename.append( *it );

        pugi::xml_document doc;
        if ( doc.load_file( filename.c_str() ))
        {
            pugi::xml_node xmlRootNode = doc.child( "EntityLibrary" );
            if (xmlRootNode)
            {
                for (pugi::xml_node_iterator it = xmlRootNode.begin(); it != xmlRootNode.end(); ++it)
                {
                    const pugi::xml_node& xmlEntityNode = *it;

                    pugi::xml_attribute xmlAttributeKey = xmlEntityNode.attribute( "id" );
                    pugi::xml_attribute xmlAttributeType = xmlEntityNode.attribute( "type" );
                    pugi::xml_attribute xmlAttributeName = xmlEntityNode.attribute( "name" );

                    std::string key( xmlAttributeKey.value() );
                    std::string typeString( xmlAttributeType.value() );
                    std::string name( xmlAttributeName.value() );

                    CStringID::IDType keyCrc;
                    StringUtils::StringToType( key.c_str() , keyCrc );
                    CStringID entityKey( keyCrc );

                    CStringID::IDType typeCrc;
                    StringUtils::StringToType( typeString.c_str() , typeCrc );
                    CStringID entityType( typeCrc );

                    CEntity* entity = CEntityRegistry::Get()->RegisterNewEntity( entityType, entityKey );
                    entity->SetName( name );

                    CEntity::TComponentMap& components = entity->GetComponents();
                    FOR_EACH( CEntity::TComponentMap, it, components )
                    {
                        IEntityComponent* component = it->second;

                        pugi::xml_node componentXmlNode = xmlEntityNode.child( component->GetName() );

                        CDbObject* dbObject = component;

                        dbObject->GetDescriptor().Load( dbObject, &componentXmlNode );
                    }
                }
            }
        }
    }
}

void CEntityLibrary::SaveNewPrototype(CEntity* newEntity)
{
	std::vector<std::string> listFiles;
	GetEntityLibraryFilePaths(listFiles);

	FOR_EACH_CONST(std::vector<std::string>, it, listFiles)
	{
		std::string filename;
		PathHelpers::GetEntitiesPath(filename);
		filename.append(*it);

		pugi::xml_document doc;
		if (doc.load_file(filename.c_str()))
		{
			pugi::xml_node xmlRootNode = doc.child("EntityLibrary");
			if (xmlRootNode)
			{
				pugi::xml_node entityXmlNode = xmlRootNode.append_child("Entity");
				entityXmlNode.append_attribute("id") = newEntity->GetKey().GetUniqueID();
				entityXmlNode.append_attribute("name") = newEntity->GetName();
				entityXmlNode.append_attribute("type") = newEntity->GetType().GetUniqueID();

				const CEntity::TComponentMap& components = newEntity->GetComponents();
				FOR_EACH_CONST(CEntity::TComponentMap, it, components)
				{
					const IEntityComponent* component = it->second;

					pugi::xml_node componentXmlNode = entityXmlNode.append_child(component->GetName());

					const CDbObject* dbObjectConst = component;
					dbObjectConst->GetDescriptor().Save(dbObjectConst, &componentXmlNode);
				}
			}
			doc.save_file(filename.c_str());
			return;
		}
	}
}


void CEntityLibrary::SaveAll()
{
	std::vector<std::string> listFiles;
	GetEntityLibraryFilePaths(listFiles);

	FOR_EACH_CONST(std::vector<std::string>, it, listFiles)
	{
		std::string filename;
		PathHelpers::GetEntitiesPath(filename);
		filename.append(*it);

		pugi::xml_document doc;
		pugi::xml_node xmlEntityPrototypeNode = doc.append_child();
		xmlEntityPrototypeNode.set_name("EntityLibrary");

		FOR_EACH_CONST(TEntityMap, itEntity, m_entityMap)
		{
			CEntity* entityPrototype = itEntity->second;
			if (xmlEntityPrototypeNode)
			{
				pugi::xml_node entityXmlNode = xmlEntityPrototypeNode.append_child("Entity");
				entityXmlNode.append_attribute("id") = entityPrototype->GetKey().GetUniqueID();
				entityXmlNode.append_attribute("name") = entityPrototype->GetName();
				entityXmlNode.append_attribute("type") = entityPrototype->GetType().GetUniqueID();

				const CEntity::TComponentMap& components = entityPrototype->GetComponents();
				FOR_EACH_CONST(CEntity::TComponentMap, it, components)
				{
					const IEntityComponent* component = it->second;

					pugi::xml_node componentXmlNode = entityXmlNode.append_child(component->GetName());

					const CDbObject* dbObjectConst = component;
					dbObjectConst->GetDescriptor().Save(dbObjectConst, &componentXmlNode);
				}
			}
		}

		doc.save_file(filename.c_str());
	}
}

CEntity* CEntityLibrary::CreateNewEntityFrom(CEntity* sourceEntity)
{
	CStringID entityType = sourceEntity->GetType();
	std::string entityName(sourceEntity->GetName());

	entityName.append("_");
	U32 nextEntitySufix = CLevel::Get()->GetNextEntityCountSufix(entityType);
	entityName.append(StringUtils::TypeToString(nextEntitySufix));

	CEntity* newEntity = CEntityRegistry::Get()->CreateNewEntity(entityType, CStringID(entityName.c_str()));
	newEntity->SetName(entityName);

	ParseDataFromEntity(sourceEntity, newEntity);

	return newEntity;
}

void CEntityLibrary::ParseDataFromEntity(CEntity* sourceEntity, CEntity* targetEntity)
{
	pugi::xml_document doc;
	pugi::xml_node libraryObjectNode = doc.append_child("EntityTemp");
	ExportToXMLNode(sourceEntity, libraryObjectNode);
	ParseFromXMLNode(targetEntity, libraryObjectNode);
}

CEntity* CEntityLibrary::RegisterPrototypeFrom(CEntity* sourceEntity)
{
	CStringID entityType = sourceEntity->GetType();
	std::string entityName(sourceEntity->GetName());

	entityName.append("_");
	U32 nextEntitySufix = CLevel::Get()->GetNextEntityCountSufix(entityType);
	entityName.append(StringUtils::TypeToString(nextEntitySufix));

	CEntity* newEntity = CEntityRegistry::Get()->CreateNewEntity(entityType, CStringID(entityName.c_str()));

	newEntity->SetName(entityName);

	ParseDataFromEntity(sourceEntity, newEntity);

	//register entity
	m_entityMap[newEntity->GetKey().GetUniqueID()] = newEntity;

	SaveNewPrototype(newEntity);

	return newEntity;
}

void CEntityLibrary::ExportToXMLNode(CEntity* entity, pugi::xml_node& libraryObjectNode)
{
	libraryObjectNode.set_name("Entity");
	libraryObjectNode.append_attribute("name") = entity->GetName();
	libraryObjectNode.append_attribute("id") = StringUtils::TypeToString(entity->GetKey().GetUniqueID()).c_str();
	libraryObjectNode.append_attribute("type") = StringUtils::TypeToString(entity->GetType().GetUniqueID()).c_str();
	libraryObjectNode.append_attribute("pos") = StringUtils::TypeToString(entity->GetPos()).c_str();

	if (entity->HasParent())
	{
		libraryObjectNode.append_attribute("parentID") = StringUtils::TypeToString(entity->GetParent().GetUniqueID()).c_str();
	}

	const CEntity::TComponentMap& components = entity->GetComponents();
	FOR_EACH_CONST(CEntity::TComponentMap, it, components)
	{
		const IEntityComponent* component = it->second;

		pugi::xml_node componentXmlNode = libraryObjectNode.append_child(component->GetName());

		const CDbObject* dbObjectConst = component;

		dbObjectConst->GetDescriptor().Save(dbObjectConst, &componentXmlNode);
	}
}

void CEntityLibrary::ParseFromXMLNode(CEntity* entityTarget, pugi::xml_node& xmlEntityNode)
{
	pugi::xml_attribute xmlAttributeKey = xmlEntityNode.attribute("id");
	pugi::xml_attribute xmlAttributeType = xmlEntityNode.attribute("type");
	pugi::xml_attribute xmlAttributeName = xmlEntityNode.attribute("name");

	std::string key(xmlAttributeKey.value());
	std::string typeString(xmlAttributeType.value());
	std::string name(xmlAttributeName.value());

	CStringID::IDType keyCrc;
	StringUtils::StringToType(key.c_str(), keyCrc);
	CStringID entityKey(keyCrc, name.c_str());

	CStringID::IDType typeCrc;
	StringUtils::StringToType(typeString.c_str(), typeCrc);
	CStringID entityType(typeCrc);

	CEntity::TComponentMap& components = entityTarget->GetComponents();
	FOR_EACH(CEntity::TComponentMap, it, components)
	{
		IEntityComponent* component = it->second;

		pugi::xml_node componentXmlNode = xmlEntityNode.child(component->GetName());

		CDbObject* dbObject = component;

		dbObject->GetDescriptor().Load(dbObject, &componentXmlNode);
	}
}

CEntity* CEntityLibrary::GetEntity(CStringID entityKey) const
{
	TEntityMap::const_iterator it = m_entityMap.find(entityKey.GetUniqueID());
	if (it != m_entityMap.end())
	{
		return it->second;
	}

	return NULL;
}

void CEntityLibrary::GetEntityLibraryFilePaths(std::vector<std::string>& files_out)
{
	std::string filename;
	PathHelpers::GetEntitiesPath(filename);
	FileHelpers::GetFilesInDirectory(filename.c_str(), "xml", files_out);
}