#include "Editor_PCH.h"
#include "Editor/DB/PropertyModifierString.h"

StringPropertyModifier::StringPropertyModifier(const wxString& label)
{
	m_property = new wxStringProperty(label, GetPropertyName(), "");
	m_visitor = new CGenericVisitor<std::string>();
}

StringPropertyModifier::~StringPropertyModifier()
{
	delete m_visitor;
}

void StringPropertyModifier::ApplyPropertyToMember()
{
	//get property value
	wxString value = m_property->GetValue().GetString();

	//add it to the member
	*(m_visitor->m_value) = std::string(value.c_str());
}

void StringPropertyModifier::ApplyMemberToProperty()
{
	//get member value;
	std::string& value = *(m_visitor->m_value);

	//add it to the property
	m_property->SetValueFromString(wxString(value.c_str()));
}

wxPGProperty* StringPropertyModifier::GetProperty() const { return m_property; }