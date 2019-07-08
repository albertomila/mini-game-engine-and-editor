#include "Game_PCH.h"
#include "Game/DB/DBManager.h"
#include "Game/DB/DBObjectDescriptor.h"
#include "Game/Serialization/Member.h"
#include "Game/libs/pugixml/pugixml.hpp"

CDbObjectDescriptor::CDbObjectDescriptor()
	: m_isRegistered(false)
{
}

void CDbObjectDescriptor::PushMember(CMember* member)
{
	m_memberList.push_back(member);
}

void CDbObjectDescriptor::Load( CDbObject* dbObject, const pugi::xml_node* xmlNode) const
{
	FOR_EACH_CONST( TMemberList, it, m_memberList )
	{
		(*it)->Load( dbObject, xmlNode );
	}
}

void CDbObjectDescriptor::Save( const CDbObject* dbObject, pugi::xml_node* xmlNode ) const
{
	FOR_EACH_CONST( TMemberList, it, m_memberList )
	{
		(*it)->Save( dbObject, xmlNode );
	}
}
