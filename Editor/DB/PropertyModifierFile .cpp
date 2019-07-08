#include "Editor_PCH.h"
#include "Editor/DB/PropertyModifierFile.h"

FilePropertyModifier::FilePropertyModifier(const wxString& label)
{
	m_property = new wxFileProperty(label, GetPropertyName(), "");
	m_visitor = new CGenericVisitor<CDBFile>();
}

FilePropertyModifier::~FilePropertyModifier()
{
	delete m_visitor;
}

void FilePropertyModifier::ApplyPropertyToMember()
{
	//get property value
	wxString value = m_property->GetValue().GetString();

	std::string relativePath;
	PathHelpers::GetRelativePath(value.c_str(), relativePath);
	m_property->SetValueFromString(wxString(relativePath.c_str()));

	//add it to the member
	*(m_visitor->m_value) = CDBFile(relativePath);
}

void FilePropertyModifier::ApplyMemberToProperty()
{
	//get member value;
	CDBFile& value = *(m_visitor->m_value);

	//add it to the property
	m_property->SetValueFromString(wxString(value.m_filename.c_str()));
}

wxPGProperty* FilePropertyModifier::GetProperty() const { return m_property; }