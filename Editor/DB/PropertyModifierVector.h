#pragma once
#include "Editor/DB/PropertyModifierHelpers.h"

class VectorPropertyModifier : public IPropertyModifier
{
public:
	VectorPropertyModifier(const wxString& label = wxPG_LABEL);

	virtual ~VectorPropertyModifier();
	virtual void ApplyPropertyToMember();
	virtual void ApplyMemberToProperty();
	void AddNewEntry();
	S32 RemoveEntry(CDbObject* dbObject);
	CDBObjectVectorVisitor*   GetTypeVisitor() const { return m_visitor; }
	virtual wxPropertyCategory* GetProperty() const;
	std::vector<CDbObject*>& GetDBObjectList() const;

private:
	wxPropertyCategory*         m_property;
	CDBObjectVectorVisitor*     m_visitor;
};
