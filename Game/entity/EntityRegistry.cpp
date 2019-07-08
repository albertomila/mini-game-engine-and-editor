#include "Game_PCH.h"
#include "Game/Entity/EntityRegistry.h"

CEntityRegistry::CEntityRegistry() 
{
}

CEntityRegistry::~CEntityRegistry()
{
	DeleteMap(m_entityTypesMap);
}

CEntity* CEntityRegistry::CreateNewEntity(CStringID idEntityType, CStringID newEntityName)
{
	TEntityTypesMap::iterator it = m_entityTypesMap.find(idEntityType.GetUniqueID());
	if (it != m_entityTypesMap.end())
	{
		ITypeEntity* typeEntity = it->second;
		CEntity* newEntity = typeEntity->CreateNewEntity(newEntityName);
		newEntity->SetType(idEntityType);

		return newEntity;
	}

	//throw error
	return NULL;
}

CEntity* CEntityRegistry::RegisterNewEntity(CStringID idEntityType, CStringID newEntityName)
{
	TEntityTypesMap::iterator it = m_entityTypesMap.find(idEntityType.GetUniqueID());
	if (it != m_entityTypesMap.end())
	{
		ITypeEntity* typeEntity = it->second;
		CEntity* newEntity = typeEntity->RegisterNewEntity(newEntityName);
		newEntity->SetType(idEntityType);

		return newEntity;
	}

	//throw error
	return NULL;
}
