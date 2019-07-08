#include "Editor_PCH.h"
#include "Editor/DB/PropertyModifierVector3.h"

Vector3PropertyModifier::Vector3PropertyModifier(const wxString& label)
	: m_x(NULL)
	, m_y(NULL)
	, m_z(NULL)
{
	m_property = new wxPropertyCategory(label, GetPropertyName());
	m_property->SetExpanded(false);

	m_x = new wxFloatProperty(wxString("x"), SPropertyIDHelpers::GetNextPropertyName(), 0);
	m_y = new wxFloatProperty(wxString("y"), SPropertyIDHelpers::GetNextPropertyName(), 0);
	m_z = new wxFloatProperty(wxString("z"), SPropertyIDHelpers::GetNextPropertyName(), 0);

	m_visitor = new CGenericVisitor<vector3>();
}

Vector3PropertyModifier::~Vector3PropertyModifier()
{
}

void Vector3PropertyModifier::ApplyPropertyToMember()
{
	//get property value
	double valueX = m_x->GetValue().GetDouble();
	double valueY = m_y->GetValue().GetDouble();
	double valueZ = m_z->GetValue().GetDouble();

	//add it to the member
	*(m_visitor->m_value) = vector3(valueX, valueY, valueZ);
}

void Vector3PropertyModifier::ApplyMemberToProperty()
{
	//get member value;
	vector3& value = *(m_visitor->m_value);

	//add it to the property
	m_x->SetValue(value[VECT_X]);
	m_y->SetValue(value[VECT_Y]);
	m_z->SetValue(value[VECT_Z]);
}

wxPropertyCategory* Vector3PropertyModifier::GetProperty() const { return m_property; }
wxFloatProperty* Vector3PropertyModifier::GetPropertyX() const { return m_x; }
wxFloatProperty* Vector3PropertyModifier::GetPropertyY() const { return m_y; }
wxFloatProperty* Vector3PropertyModifier::GetPropertyZ() const { return m_z; }