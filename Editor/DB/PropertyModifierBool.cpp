#include "Editor_PCH.h"
#include "Editor/DB/PropertyModifierBool.h"

BoolPropertyModifier::BoolPropertyModifier(const wxString& label)
{
	m_property = new wxBoolProperty(label, GetPropertyName(), false);
	m_visitor = new  CGenericVisitor<bool>();
}

BoolPropertyModifier::~BoolPropertyModifier()
{
	delete m_visitor;
}

void BoolPropertyModifier::ApplyPropertyToMember()
{
	//get property value
	bool value = m_property->GetValue().GetBool();

	//add it to the member
	*(m_visitor->m_value) = value;
}

void BoolPropertyModifier::ApplyMemberToProperty()
{
	//get member value;
	bool value = *(m_visitor->m_value);
	wxVariant variantValue(value);

	//add it to the property
	m_property->SetValue(variantValue);
}

wxPGProperty* BoolPropertyModifier::GetProperty() const { return m_property; }