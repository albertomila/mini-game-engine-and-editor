#pragma once
#include "Editor/DB/PropertyModifierHelpers.h"

class Vector2PropertyModifier : public IPropertyModifier
{
public:
	Vector2PropertyModifier(const wxString& label = wxPG_LABEL);
	virtual ~Vector2PropertyModifier();
	virtual void ApplyPropertyToMember();
	virtual void ApplyMemberToProperty();
	ITypeVisitor*   GetTypeVisitor() const { return m_visitor; }
	virtual wxPropertyCategory* GetProperty() const;
	virtual wxFloatProperty* GetPropertyX() const;
	virtual wxFloatProperty* GetPropertyY() const;

private:
	CGenericVisitor<vector2>* m_visitor;

	wxPropertyCategory*     m_property;
	wxFloatProperty*        m_x;
	wxFloatProperty*        m_y;
};