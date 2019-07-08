#pragma once


#include "Editor/DB/PropertyModifierHelpers.h"

////////////////////////////////////////////////////////////////////////////
//FloatPropertyModifier
////////////////////////////////////////////////////////////////////////////
template<class FLOAT_TYPE>
class FloatPropertyModifier : public IPropertyModifier
{
public:
	FloatPropertyModifier(const wxString& label = wxPG_LABEL)
	{
		m_property = new wxFloatProperty(label, GetPropertyName(), 0);
		m_visitor = new  CGenericVisitor<FLOAT_TYPE>();
	}

	virtual ~FloatPropertyModifier()
	{
		delete m_visitor;
	}

	virtual void ApplyPropertyToMember()
	{
		//get property value
		double value = m_property->GetValue().GetDouble();

		//add it to the member
		*(m_visitor->m_value) = static_cast<FLOAT_TYPE>(value);
	}

	virtual void ApplyMemberToProperty()
	{
		//get member value;
		FLOAT_TYPE& value = *(m_visitor->m_value);
		wxVariant variantValue(value);

		//add it to the property
		m_property->SetValue(variantValue);
	}


	ITypeVisitor*   GetTypeVisitor() const { return m_visitor; }
	virtual wxPGProperty* GetProperty() const { return m_property; }
private:
	wxFloatProperty*       m_property;
	CGenericVisitor<FLOAT_TYPE>* m_visitor;
};
