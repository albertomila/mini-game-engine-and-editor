#pragma once
#include "Game/Core/DynamicObject.h"
#include "Game/DB/DBObjectDescriptor.h"
#include "Game/GameDefs.h"
#include "Game/libs/pugixml/pugixml.hpp"
#include <string>

/////////////////////////////////////////////////////////////////////////////////
//CDbObject declare macro
/////////////////////////////////////////////////////////////////////////////////
#define DECLARE_DB_OBJECT( CLASS_TYPE, PARENT_CLASS_TYPE )						\
		DECLARE_DYNAMIC_OBJECT( CLASS_TYPE, PARENT_CLASS_TYPE );				\
public:																			\
    virtual CDbObject* CreateDerivedObject(){ return new CLASS_TYPE; }		    \
	static CDbObject* CreateObject(){ return new CLASS_TYPE; }					\
	virtual CDbObjectDescriptor& GetDescriptor() override			\
	{																			\
		return ms_descriptor;													\
    }																		\
    virtual const CDbObjectDescriptor& GetDescriptor() const override			\
    {																			\
        return ms_descriptor;													\
    }																		\
	virtual void Register() override											\
	{																			\
		RegisterProperties();													\
	}																			\
	static void RegisterProperties();                                           \
private:																        \
	static void RegisterParent()                                                \
	{                                                                           \
		super::RegisterProperties();                                            \
	}                                                                           \
protected:                                                                      \
	static CDbObjectDescriptor ms_descriptor;									\
private:																		\

/////////////////////////////////////////////////////////////////////////////////
//CDbObject implement macro
/////////////////////////////////////////////////////////////////////////////////
#define BEGIN_DEFINE_DB_OBJECT( CLASS_TYPE )		  \
CDbObjectDescriptor CLASS_TYPE::ms_descriptor;        \
void CLASS_TYPE::RegisterProperties()                 \
{													  \
	if (ms_descriptor.IsRegistered() )				  \
		return;										  \
	RegisterParent();								  \
	ms_descriptor.SetRegistered();					  \
    CDbObjectDescriptor::TMemberList& ms_members = ms_descriptor.GetMemberList();

#define END_DEFINE_DB_OBJECT()		      \
}		                                      

#define SERIALIZE_MEMBER( MEMBER_NAME, MEMBER, MEMBER_HANDLER ) \
	ms_members.push_back( MemberCreator::Create<MEMBER_HANDLER>( MEMBER_NAME, &self::MEMBER ) );


#define BEGIN_SERIALIZE_MEMBER_ENUM( MEMBER_NAME, MEMBER )                         \
    ms_members.push_back( MemberCreator::CreateEnumMember<BasicTypeHandlerEnum>( MEMBER_NAME, &self::MEMBER ) );  \
    CEnumMember * enumMember = new CEnumMember( MEMBER_NAME );                                          \
    ms_members.push_back( enumMember );  


#define REGISTER_ENUM_VALUE( ENUM_VALUE_NAME, ENUM_VALUE )                                            \
    enumMember->PushEnumValue( ENUM_VALUE_NAME, ENUM_VALUE );

#define END_SERIALIZE_MEMBER_ENUM()

#define BEGIN_GROUP( GROUPNAME )                                 \
{                                                                \
    CGroupMember* groupMember = new CGroupMember( GROUPNAME );   \
    ms_members.push_back( groupMember );                 \
    {                                                            \
        CDbObjectDescriptor::TMemberList& ms_members = groupMember->GetMemberList();

#define END_GROUP() \
    } \
}
/////////////////////////////////////////////////////////////////////////////////
//CDbObject
/////////////////////////////////////////////////////////////////////////////////
class GAME_DLL CDbObject : public CDynamicObject
{
	DECLARE_DYNAMIC_OBJECT( CDbObject, CDynamicObject );
public:			
	CDbObject(){}
	virtual ~CDbObject(){}

    virtual CDbObject* CreateDerivedObject() = 0;
    virtual CDbObjectDescriptor& GetDescriptor() = 0;
    virtual const CDbObjectDescriptor& GetDescriptor() const = 0;
	virtual void Register() = 0;
	static void RegisterProperties(){}

	CStringID GetKey() const{ return m_key; }
	const char* GetName() const{ return m_name.c_str(); }

	void SetKey( CStringID key ){ m_key = key; }
	void SetName( const char* name ){ m_name = name; }

protected:                                                                      
	static CDbObjectDescriptor ms_descriptor;
	CStringID	m_key;
	std::string m_name;
};