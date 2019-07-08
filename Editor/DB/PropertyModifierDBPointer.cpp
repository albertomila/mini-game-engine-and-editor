#include "Editor_PCH.h"
#include "Editor/DB/PropertyModifierDBPointer.h"

DBObjectPtrPropertyModifier::DBObjectPtrPropertyModifier(const wxString& label)
	{
		m_property = new wxStringProperty(label, GetPropertyName(), "");
		m_visitor = new CDBObjectPointerVisitor();
	}

	DBObjectPtrPropertyModifier::~DBObjectPtrPropertyModifier()
	{
		delete m_visitor;
	}

	void DBObjectPtrPropertyModifier::ApplyPropertyToMember()
	{
		const CDBManager::TLibraryMap* libMap = CDBManager::Get()->GetLibrary(m_visitor->m_libraryID);
		if (libMap)
		{
			const CDBManager::TObjMapConstIt itObj = libMap->find(m_visitor->m_objectValue.GetUniqueID());
			if (itObj != libMap->end())
			{
				CDbObject* dbObjectSelected = itObj->second;
				*(m_visitor->m_dbObject) = dbObjectSelected;
			}
		}
	}

	void DBObjectPtrPropertyModifier::ApplyMemberToProperty()
	{
		UpdateDisplayText();
	}

	wxStringProperty* DBObjectPtrPropertyModifier::GetProperty() const { return m_property; }

	void DBObjectPtrPropertyModifier::OnItemSelected(CStringID objectSelectedID)
	{
		m_visitor->m_objectValue = objectSelectedID;
		UpdateDisplayText();
	}

	void DBObjectPtrPropertyModifier::UpdateDisplayText()
	{
		if (m_visitor->m_dbObject)
		{
			wxString libraryObjectDisplayText = CDBManager::Get()->GetLibraryPrefix(m_visitor->m_libraryID);
			libraryObjectDisplayText.Append(".");
			//            libraryObjectDisplayText.Append( m_visitor->m_objectValue.c_str() );
			m_property->SetValueFromString(libraryObjectDisplayText);
		}
	}