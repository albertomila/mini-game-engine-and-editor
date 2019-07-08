#include "Editor_PCH.h"
#include "Editor/DB/PropertyModifierVector.h"

VectorPropertyModifier::VectorPropertyModifier(const wxString& label)
{
	m_property = new wxPropertyCategory(label, GetPropertyName());
	m_property->SetExpanded(false);
	m_visitor = new CDBObjectVectorVisitor();
}

VectorPropertyModifier::~VectorPropertyModifier()
{
	delete m_visitor;
}


void VectorPropertyModifier::ApplyPropertyToMember()
{
	m_visitor->m_onDataChangedCallback.Call(m_visitor);
}

void VectorPropertyModifier::ApplyMemberToProperty()
{
}

void VectorPropertyModifier::AddNewEntry()
{
	m_visitor->m_onAddNewEntryCallback.Call(m_visitor);
}

S32 VectorPropertyModifier::RemoveEntry(CDbObject* dbObject)
{
	S32 index(-1);
	FOR_EACH_SLOW(std::vector<CDbObject*>, it, m_visitor->m_dbObjectList)
	{
		++index;
		if (dbObject == *it)
		{
			it = m_visitor->m_dbObjectList.erase(it);
			//return if found

			return index;
		}
		else
		{
			++it;
		}
	}

	return index;

	//error if not found.
}

wxPropertyCategory* VectorPropertyModifier::GetProperty() const
{
	return m_property;
}

std::vector<CDbObject*>& VectorPropertyModifier::GetDBObjectList() const
{
	return m_visitor->m_dbObjectList;
}