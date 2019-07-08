#pragma once
#include "Editor/DB/PropertyModifierHelpers.h"
#include "Editor/DB/PropertyModifierDBObject.h"

class DBObjectPtrPropertyModifier : public IPropertyModifier
{
public:
	DBObjectPtrPropertyModifier(const wxString& label = wxPG_LABEL);
	virtual ~DBObjectPtrPropertyModifier();
	virtual void ApplyPropertyToMember();
	virtual void ApplyMemberToProperty();
	CDBObjectPointerVisitor* GetTypeVisitor() const { return m_visitor; }
	virtual wxStringProperty* GetProperty() const;
	void OnItemSelected(CStringID objectSelectedID);

private:
	void UpdateDisplayText();

	wxStringProperty*           m_property;
	CDBObjectPointerVisitor*    m_visitor;
};
