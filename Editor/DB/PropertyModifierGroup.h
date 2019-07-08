#pragma once
#include "Editor/DB/PropertyModifierHelpers.h"

class GroupPropertyModifier : public IPropertyModifier
{
public:
	GroupPropertyModifier(const CMember* member, const wxString& label = wxPG_LABEL);
	virtual ~GroupPropertyModifier();
	virtual void ApplyPropertyToMember();
	virtual void ApplyMemberToProperty();
	ITypeVisitor*   GetTypeVisitor() const { return NULL; }
	virtual wxPropertyCategory* GetProperty() const;
	const CGroupMember::TMemberList& GetMemberList() const;
private:
	const CGroupMember*        m_groupMember;
	wxPropertyCategory*          m_property;
};