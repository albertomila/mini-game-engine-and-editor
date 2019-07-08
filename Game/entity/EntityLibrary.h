#pragma once

#include <Game/Core/Singleton.h>
#include <Game/Core/Traits.h>
#include <Game/Entity/Entity.h>
#include <Game/libs/pugixml/pugixml.hpp>
#include <unordered_map>

namespace EntityComponentCreator
{
    //////////////////////////////////////////SIZE DE LE LISTE DE TYPES
    template <class>
	struct SComponentCreator
	{
		static void CreateComponent(CEntity* entity)
		{
		}
	};

    template <>
    struct SComponentCreator<SEmpty>
    {
        static void CreateComponent( CEntity* entity )
        {
        }
    };

    template <class T, class Q>
    struct SComponentCreator< STypeList<T, Q> >
    {
        static void CreateComponent( CEntity* entity )
        {
            entity->RegisterComponent<T>();

            //create next component
            SComponentCreator<Q>::CreateComponent( entity );
        }
    };
}

class GAME_DLL CEntityLibrary : public CSingleton<CEntityLibrary>
{
public:
    typedef  std::unordered_map< CStringID::IDType, CEntity* > TEntityMap;

	CEntityLibrary();
	~CEntityLibrary();

    template< class TComponentList >
	CEntity* CreateEntity(CStringID entityID);

	template< class TComponentList >
	CEntity* RegisterEntity(CStringID entityID);

	CEntity* CreateNewEntityFrom(CEntity* sourceEntity);
	void ParseDataFromEntity(CEntity* sourceEntity, CEntity* targetEntity);
	CEntity* RegisterPrototypeFrom(CEntity* sourceEntity);
	void ExportToXMLNode(CEntity* entity, pugi::xml_node& libraryObjectNode);
	void ParseFromXMLNode(CEntity* entityTarget, pugi::xml_node& xmlEntityNode);
	CEntity* GetEntity(CStringID entityKey) const;
	void GetEntityLibraryFilePaths(std::vector<std::string>& files_out);

	void LoadAll();
	void SaveAll();
	void SaveNewPrototype(CEntity* entity);

	const TEntityMap& GetEntities() const { return m_entityMap; }

private:
    TEntityMap m_entityMap;
};

template< class TComponentList >
CEntity* CEntityLibrary::CreateEntity(CStringID entityID)
{
	//create entity
	CEntity* newEntity = new CEntity();
	newEntity->SetKey(entityID);

	//create components
	EntityComponentCreator::SComponentCreator<TComponentList>::CreateComponent(newEntity);

	return newEntity;
}

template< class TComponentList >
CEntity* CEntityLibrary::RegisterEntity(CStringID entityID)
{
	//create entity
	CEntity* newEntity = CreateEntity<TComponentList>(entityID);

	//register entity
	m_entityMap[entityID.GetUniqueID()] = newEntity;

	return newEntity;
}
