#include "Editor_PCH.h"
#include "Editor/DB/PropertyModifierVector2.h"

Vector2PropertyModifier::Vector2PropertyModifier(const wxString& label)
	: m_x(NULL)
	, m_y(NULL)
{
	m_property = new wxPropertyCategory(label, GetPropertyName());
	m_property->SetExpanded(false);

	m_x = new wxFloatProperty(wxString("x"), SPropertyIDHelpers::GetNextPropertyName(), 0);
	m_y = new wxFloatProperty(wxString("y"), SPropertyIDHelpers::GetNextPropertyName(), 0);

	m_visitor = new CGenericVisitor<vector2>();
}

Vector2PropertyModifier::~Vector2PropertyModifier()
{
}

void Vector2PropertyModifier::ApplyPropertyToMember()
{
	//get property value
	double valueX = m_x->GetValue().GetDouble();
	double valueY = m_y->GetValue().GetDouble();

	//add it to the member
	*(m_visitor->m_value) = vector2(valueX, valueY);
}

void Vector2PropertyModifier::ApplyMemberToProperty()
{
	//get member value;
	vector2& value = *(m_visitor->m_value);

	//add it to the property
	m_x->SetValue(value[VECT_X]);
	m_y->SetValue(value[VECT_Y]);
}

wxPropertyCategory* Vector2PropertyModifier::GetProperty() const { return m_property; }
wxFloatProperty* Vector2PropertyModifier::GetPropertyX() const { return m_x; }
wxFloatProperty* Vector2PropertyModifier::GetPropertyY() const { return m_y; }
