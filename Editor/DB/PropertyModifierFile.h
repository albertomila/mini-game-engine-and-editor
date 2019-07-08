#pragma once
#include "Editor/DB/PropertyModifierHelpers.h"

class FilePropertyModifier : public IPropertyModifier
{
public:
	FilePropertyModifier(const wxString& label = wxPG_LABEL);
	virtual ~FilePropertyModifier();
	virtual void ApplyPropertyToMember();
	virtual void ApplyMemberToProperty();
	ITypeVisitor*   GetTypeVisitor() const { return m_visitor; }
	virtual wxPGProperty* GetProperty() const;

private:
	wxFileProperty*          m_property;
	CGenericVisitor<CDBFile>* m_visitor;
};