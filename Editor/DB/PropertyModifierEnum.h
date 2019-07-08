#pragma once
#include "Editor/DB/PropertyModifierHelpers.h"

class EnumPropertyModifier : public IPropertyModifier
{
public:
	EnumPropertyModifier
	(const CDbObjectDescriptor::TMemberList& memberList
	, const CMember* member
	, const wxString& label);
	virtual ~EnumPropertyModifier();
	virtual void ApplyPropertyToMember();
	virtual void ApplyMemberToProperty();
	ITypeVisitor*   GetTypeVisitor() const { return m_visitor; }
	virtual wxPGProperty* GetProperty() const;

private:
	wxEnumProperty*         m_property;
	CGenericVisitor<U32>*   m_visitor;
};
