#pragma once
#include "Game/Core/Math.h"
#include "Game/Core/StringID.h"
#include <unordered_map>


class IEntityComponent;

///////////////////////////////////////////////////////////////////////////
//CEntity
///////////////////////////////////////////////////////////////////////////
class GAME_DLL CEntity
{
public:
    typedef std::unordered_map< CStringID::IDType, IEntityComponent*> TComponentMap;
    CEntity();
    virtual ~CEntity();

    template<class T>
    void RegisterComponent();

    template<class T>
    T* GetComponent() const;
    IEntityComponent* GetComponent( CStringID componentID ) const;

    void SetKey( CStringID keyID ){ m_keyID = keyID; }
    CStringID GetKey() const{ return m_keyID; }

	void SetName(std::string& name) { m_name = name; }
	std::string& GetNameRef() { return m_name; }
    const char* GetName() const{ return m_name.c_str(); }

    void SetType( CStringID entityType ){ m_entityType = entityType; }
    CStringID GetType() const{ return m_entityType; }

    const TComponentMap& GetComponents() const{ return m_components; }
    TComponentMap& GetComponents(){ return m_components; }

    void SetPos( const vector3& val );
    vector3 GetPos() const;

    void SetRotation( float angle );

    void SetParent( CStringID keyID ){ m_parentKeyID = keyID; }
    CStringID GetParent() const{ return m_parentKeyID; }
    bool HasParent() const{ return m_parentKeyID.IsValid(); }

    const matrix& GetMatrix() const{ return m_matrix; }

private:
    TComponentMap m_components;
    CStringID     m_keyID;
    CStringID     m_parentKeyID;
    CStringID     m_entityType;
    std::string   m_name;
    matrix        m_matrix;
};

///////////////////////////////////////////////////////////////////////////
template<class T>
void CEntity::RegisterComponent()
{
	IEntityComponent* newComponent = T::CreateComponent();
	m_components[T::GetClassType().GetUniqueID()] = newComponent;
}

///////////////////////////////////////////////////////////////////////////
template<class T>
T* CEntity::GetComponent() const
{
	TComponentMap::const_iterator itFind = m_components.find(T::GetClassType().GetUniqueID());
	if (itFind != m_components.end())
	{
		IEntityComponent* component = itFind->second;
		return T::DynamicCast(component);
	}

	return NULL;
}