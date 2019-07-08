#include "Game_PCH.h"
#include "Game/Serialization/Member.h"

///////////////////////////////////////////////////////////////////////
ITypeVisitor::~ITypeVisitor() {}

///////////////////////////////////////////////////////////////////////
ITypeVisitorPtr::~ITypeVisitorPtr() {}

///////////////////////////////////////////////////////////////////////
CDBObjectVectorVisitor::CDBObjectVectorVisitor() {}
CDBObjectVectorVisitor::~CDBObjectVectorVisitor() {}
void CDBObjectVectorVisitor::Visit(void* value)
{
	std::vector<CDbObject*>* dbObjectList = static_cast<std::vector<CDbObject*>*>(value);
	if (dbObjectList)
	{
		m_dbObjectList = *dbObjectList;
	}
}


///////////////////////////////////////////////////////////////////////
CMember::CMember(const char* memberName)
	: m_memberName(memberName)
	, m_memberID(memberName)
{
}

///////////////////////////////////////////////////////////////////////

CEnumMember::SEnumValue::SEnumValue(const char* valueName, S32 enumValue)
	: m_valueName(valueName)
	, m_enumValue(enumValue)
{
}

///////////////////////////////////////////////////////////////////////
CEnumMember::CEnumMember(const char* enumName)
	: CMember(enumName)
{
}

 void CEnumMember::Load(CDbObject* parent, const pugi::xml_node* xmlNode) const
{
}

void CEnumMember::Save(const CDbObject* parent, pugi::xml_node* xmlNode) const
{
}

void CEnumMember::PushEnumValue(const char* valueName, U32 enumValue)
{
	m_values.push_back(SEnumValue(valueName, enumValue));
}

void CEnumMember::Visit(const CDbObject* parent, ITypeVisitor* visitor)
{
}

CStringID::IDType CEnumMember::GetMemberTypeID() const
{
	return 0;
}


///////////////////////////////////////////////////////////////////////
CGroupMember::CGroupMember(const char* groupName)
	: CMember(groupName)
{
}

void CGroupMember::Load(CDbObject* parent, const pugi::xml_node* xmlNode) const
{
	pugi::xml_node xmlChildNode = xmlNode->child(m_memberName);

	if (xmlChildNode)
	{
		FOR_EACH_CONST(TMemberList, it, ms_members)
		{
			(*it)->Load(parent, &xmlChildNode);
		}
	}
}

void CGroupMember::Save(const CDbObject* parent, pugi::xml_node* xmlNode) const
{
	pugi::xml_node newXmlNode = xmlNode->append_child(m_memberName);

	FOR_EACH_CONST(TMemberList, it, ms_members)
	{
		(*it)->Save(parent, &newXmlNode);
	}
}

void CGroupMember::Visit(const CDbObject* parent, ITypeVisitor* visitor)
{
}

CMember* CGroupMember::GetChildMember(const CStringID& name, const CDbObject* parent) const
{
	FOR_EACH_CONST(TMemberList, it, ms_members)
	{
		if ((*it)->GetID() == name)
		{
			return (*it);
		}
	}

	return NULL;
}

CStringID::IDType CGroupMember::GetMemberTypeID() const
{
	return MemberToTypeID<MemberTypeIDGroup>::value;
}