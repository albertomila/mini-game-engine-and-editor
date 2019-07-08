#include "Editor_PCH.h"
#include "Editor/DB/PropertyModifierDBObject.h"

DBObjectPropertyModifier::DBObjectPropertyModifier(const wxString& label)
{
	m_property = new wxPropertyCategory(label, GetPropertyName());
	m_property->SetExpanded(false);
	m_visitor = new CGenericVisitor<CDbObject>();
}

DBObjectPropertyModifier::~DBObjectPropertyModifier()
{
	delete m_visitor;
}

void DBObjectPropertyModifier::ApplyPropertyToMember()
{
}

void DBObjectPropertyModifier::ApplyMemberToProperty()
{
	for (int i = 0; i < m_property->GetChildCount(); ++i)
	{
		wxPGProperty* propertyChild = m_property->Item(i);
		propertyChild->SetBackgroundColour(wxColour(180, 220, 255));
	}
}

CDbObject* DBObjectPropertyModifier::GetDBObject() const 
{ 
	return  m_visitor->m_value; 
}

wxPropertyCategory* DBObjectPropertyModifier::GetProperty() const 
{ 
	return m_property; 
}

CDBObjectPointerVisitor::CDBObjectPointerVisitor()
	: m_dbObject(NULL)
{
}

CDBObjectPointerVisitor::~CDBObjectPointerVisitor() {}
void CDBObjectPointerVisitor::Visit(void* value)
{
	CDbObject** dbObjectPtr = static_cast<CDbObject**>(value);
	if (dbObjectPtr)
	{
		m_dbObject = dbObjectPtr;
		if (*(m_dbObject))
		{
			m_objectValue = (*m_dbObject)->GetKey();
		}
	}
}