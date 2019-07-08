#include "Editor_PCH.h"
#include "Editor/DB/PropertyModifierGroup.h"

GroupPropertyModifier::GroupPropertyModifier(const CMember* member, const wxString& label)
{
	m_property = new wxPropertyCategory(label, GetPropertyName());
	m_property->SetExpanded(false);
	m_groupMember = static_cast<const CGroupMember*>(member);
}

GroupPropertyModifier::~GroupPropertyModifier()
{
}

void GroupPropertyModifier::ApplyPropertyToMember()
{
}

void GroupPropertyModifier::ApplyMemberToProperty()
{
	for (int i = 0; i < m_property->GetChildCount(); ++i)
	{
		wxPGProperty* propertyChild = m_property->Item(i);
		propertyChild->SetBackgroundColour(wxColour(180, 220, 255));
	}
}

wxPropertyCategory* GroupPropertyModifier::GetProperty() const
{
	return m_property;
}
const CGroupMember::TMemberList& GroupPropertyModifier::GetMemberList() const
{
	return m_groupMember->GetMemberList();
}