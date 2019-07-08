#pragma once


#include "Editor/DB/PropertyModifierHelpers.h"


////////////////////////////////////////////////////////////////////////////
//IntPropertyModifier
////////////////////////////////////////////////////////////////////////////
template<class INT_TYPE>
class IntPropertyModifier : public IPropertyModifier
{
public:
	IntPropertyModifier(const wxString& label = wxPG_LABEL)
	{
		m_property = new wxIntProperty(label, GetPropertyName(), 0);
		m_visitor = new  CGenericVisitor<INT_TYPE>();
	}

	virtual ~IntPropertyModifier()
	{
		delete m_visitor;
	}

	virtual void ApplyPropertyToMember()
	{
		//get property value
		long value = m_property->GetValue().GetInteger();

		//add it to the member
		*(m_visitor->m_value) = static_cast<INT_TYPE>(value);
	}

	virtual void ApplyMemberToProperty()
	{
		//get member value;
		INT_TYPE& value = *(m_visitor->m_value);

		//add it to the property
		m_property->SetValueFromInt(value);
	}

	ITypeVisitor*   GetTypeVisitor() const { return m_visitor; }
	virtual wxPGProperty* GetProperty() const { return m_property; }
private:
	wxIntProperty*       m_property;
	CGenericVisitor<INT_TYPE>* m_visitor;
};
