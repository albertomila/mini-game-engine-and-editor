#include "Editor_PCH.h"
#include "Editor/DB/PropertyModifierVectorElement.h"

VectorElementPropertyModifier::VectorElementPropertyModifier(const wxString& label)
{
	m_property = new wxPropertyCategory(label, GetPropertyName());
	m_property->SetExpanded(false);
}

VectorElementPropertyModifier::~VectorElementPropertyModifier()
{
}

void VectorElementPropertyModifier::ApplyPropertyToMember()
{
}

void VectorElementPropertyModifier::ApplyMemberToProperty()
{
	for (int i = 0; i < m_property->GetChildCount(); ++i)
	{
		wxPGProperty* propertyChild = m_property->Item(i);
		propertyChild->SetBackgroundColour(wxColour(180, 220, 255));
	}
}

wxPropertyCategory* VectorElementPropertyModifier::GetProperty() const 
{ 
	return m_property; 
}