#pragma once
#include "Editor/DB/PropertyModifierHelpers.h"


class ComponentPropertyModifier : public IPropertyModifier
{
public:
	ComponentPropertyModifier(const IEntityComponent* component, const wxString& label = wxPG_LABEL);
	virtual ~ComponentPropertyModifier();
	virtual void ApplyPropertyToMember();
	virtual void ApplyMemberToProperty();
	ITypeVisitor*   GetTypeVisitor() const { return NULL; }
	virtual wxPropertyCategory* GetProperty() const;
	const CGroupMember::TMemberList& GetMemberList() const;

private:
	const IEntityComponent*        m_component;
	wxPropertyCategory*          m_property;
};
