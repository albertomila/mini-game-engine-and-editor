#include "Editor_PCH.h"
#include "Editor/DB/PropertyModifierComponent.h"

ComponentPropertyModifier::ComponentPropertyModifier(const IEntityComponent* component, const wxString& label)
	: m_component(component)
{
	m_property = new wxPropertyCategory(label, GetPropertyName());
	m_property->SetExpanded(false);
}

ComponentPropertyModifier::~ComponentPropertyModifier()
{
}

void ComponentPropertyModifier::ApplyPropertyToMember()
{
}

void ComponentPropertyModifier::ApplyMemberToProperty()
{
	for (int i = 0; i < m_property->GetChildCount(); ++i)
	{
		wxPGProperty* propertyChild = m_property->Item(i);
		propertyChild->SetBackgroundColour(wxColour(180, 220, 255));
	}
}

wxPropertyCategory* ComponentPropertyModifier::GetProperty() const 
{ 
	return m_property; 
}

const CGroupMember::TMemberList& ComponentPropertyModifier::GetMemberList() const
{
	const CDbObjectDescriptor& dbDescriptor = m_component->GetDescriptor();
	return dbDescriptor.GetMemberList();
}
