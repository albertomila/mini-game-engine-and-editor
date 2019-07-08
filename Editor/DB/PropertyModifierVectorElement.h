#pragma once

#include "Editor/DB/PropertyModifierHelpers.h"

class VectorElementPropertyModifier : public IPropertyModifier
{
public:
	VectorElementPropertyModifier(const wxString& label = wxPG_LABEL);
	virtual ~VectorElementPropertyModifier();
	virtual void ApplyPropertyToMember();
	virtual void ApplyMemberToProperty();
	ITypeVisitor*   GetTypeVisitor() const { return NULL; }
	virtual wxPropertyCategory* GetProperty() const;

private:
	wxPropertyCategory*          m_property;
};