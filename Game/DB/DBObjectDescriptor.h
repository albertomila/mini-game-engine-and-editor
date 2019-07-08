#pragma once
#include <vector>

class CDbObject;
class CMember;
namespace pugi
{
	class xml_node;
}

class GAME_DLL CDbObjectDescriptor
{
public:
    typedef std::vector< CMember* > TMemberList;

	CDbObjectDescriptor();
	void Test(){}
	void Load( CDbObject* dbObject, const pugi::xml_node* xmlNode) const;
	void Save( const CDbObject* dbObject, pugi::xml_node* xmlNode ) const;
	void PushMember(CMember* member);
    TMemberList& GetMemberList(){ return m_memberList; }
    const TMemberList& GetMemberList() const { return m_memberList; }
	bool IsRegistered() const{ return m_isRegistered; }
	void SetRegistered(){ m_isRegistered = true; }

private:
	TMemberList m_memberList;
	bool		m_isRegistered;
};