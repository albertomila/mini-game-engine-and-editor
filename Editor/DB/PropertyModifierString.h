#pragma once
#include "Editor/DB/PropertyModifierHelpers.h"

class StringPropertyModifier : public IPropertyModifier
{
public:
	StringPropertyModifier(const wxString& label = wxPG_LABEL);
	virtual ~StringPropertyModifier();
	virtual void ApplyPropertyToMember();
	virtual void ApplyMemberToProperty();
	ITypeVisitor*   GetTypeVisitor() const { return m_visitor; }
	virtual wxPGProperty* GetProperty() const;

private:
	wxStringProperty*          m_property;
	CGenericVisitor<std::string>* m_visitor;
};
