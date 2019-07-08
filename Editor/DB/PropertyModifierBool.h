#pragma once
#include "Editor/DB/PropertyModifierHelpers.h"

class BoolPropertyModifier : public IPropertyModifier
{
public:
	BoolPropertyModifier(const wxString& label = wxPG_LABEL);
	virtual ~BoolPropertyModifier();
	virtual void ApplyPropertyToMember();
	virtual void ApplyMemberToProperty();
	ITypeVisitor*   GetTypeVisitor() const { return m_visitor; }
	virtual wxPGProperty* GetProperty() const;
private:
	wxBoolProperty*          m_property;
	CGenericVisitor<bool>* m_visitor;
};