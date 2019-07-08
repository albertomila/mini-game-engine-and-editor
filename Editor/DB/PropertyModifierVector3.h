#pragma once

#include "Editor/DB/PropertyModifierHelpers.h"

class Vector3PropertyModifier : public IPropertyModifier
{
public:
	Vector3PropertyModifier(const wxString& label = wxPG_LABEL);
	virtual ~Vector3PropertyModifier();
	virtual void ApplyPropertyToMember();
	virtual void ApplyMemberToProperty();
	ITypeVisitor*   GetTypeVisitor() const { return m_visitor; }
	virtual wxPropertyCategory* GetProperty() const;
	virtual wxFloatProperty* GetPropertyX() const;
	virtual wxFloatProperty* GetPropertyY() const;
	virtual wxFloatProperty* GetPropertyZ() const;

private:
	CGenericVisitor<vector3>* m_visitor;

	wxPropertyCategory*     m_property;
	wxFloatProperty*        m_x;
	wxFloatProperty*        m_y;
	wxFloatProperty*        m_z;
};
