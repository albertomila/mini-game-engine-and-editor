#pragma once
#include "Editor/DB/PropertyModifierHelpers.h"


class DBObjectPropertyModifier : public IPropertyModifier
{
public:
	DBObjectPropertyModifier(const wxString& label = wxPG_LABEL);

	virtual ~DBObjectPropertyModifier();
	virtual void ApplyPropertyToMember();
	virtual void ApplyMemberToProperty();
	CDbObject* GetDBObject() const;
	ITypeVisitor*   GetTypeVisitor() const { return m_visitor; }
	virtual wxPropertyCategory* GetProperty() const;

private:
	wxPropertyCategory*          m_property;
	CGenericVisitor<CDbObject>* m_visitor;
};

class CDBObjectPointerVisitor : public ITypeVisitorPtr
{
public:
	CDBObjectPointerVisitor();
	virtual ~CDBObjectPointerVisitor();
	virtual void Visit(void* value);

	CStringID m_objectValue;
	CDbObject** m_dbObject;
};