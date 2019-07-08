#pragma once
#include "Game\Core\Singleton.h"
#include "Game\Core\traits.h"
#include "Game\entity\Entity.h"
#include <unordered_map>
#include "Game\entity\EntityLibrary.h"

class GAME_DLL CEntityRegistry : public CSingleton<CEntityRegistry>
{
public:
    class ITypeEntity
    {
	public:
		ITypeEntity()
		{
		}

        ITypeEntity( CStringID entityType)
            : m_entityType( entityType )
        {
        }
		virtual CEntity* CreateNewEntity(CStringID newEntityName) = 0;
		virtual CEntity* RegisterNewEntity(CStringID newEntityName) = 0;

        CStringID GetEntityType() const { return m_entityType; }

    private:
        CStringID m_entityType;
    };

	template<class T>
	class STypeEntity : public ITypeEntity
	{
	public:
		STypeEntity()
		{
		}

		STypeEntity(CStringID entityType)
			: ITypeEntity(entityType)
		{
		}

		virtual CEntity* CreateNewEntity(CStringID newEntityName);
		virtual CEntity* RegisterNewEntity(CStringID newEntityName);
	};


    typedef  std::unordered_map< CStringID::IDType, ITypeEntity* > TEntityTypesMap;

	CEntityRegistry();
	~CEntityRegistry();

    template< class EntityType >
	void RegisterEntityType(CStringID idType)
	{
		m_entityTypesMap[idType.GetUniqueID()] = new STypeEntity<EntityType>(idType);
	}


	CEntity* CreateNewEntity(CStringID idEntityType, CStringID newEntityName);
	CEntity* RegisterNewEntity(CStringID idEntityType, CStringID newEntityName);
    const TEntityTypesMap& GetEntityTypes() const{ return m_entityTypesMap; }

private:
	TEntityTypesMap m_entityTypesMap;
};


template<class T>
CEntity* CEntityRegistry::STypeEntity<T>::CreateNewEntity(CStringID newEntityName)
{
	return CEntityLibrary::Get()->CreateEntity<T>(newEntityName);
}

template<class T>
CEntity* CEntityRegistry::STypeEntity<T>::RegisterNewEntity(CStringID newEntityName)
{
	return CEntityLibrary::Get()->RegisterEntity<T>(newEntityName);
}
