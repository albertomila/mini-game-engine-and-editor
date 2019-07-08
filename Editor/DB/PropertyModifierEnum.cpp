#include "Editor_PCH.h"
#include "Editor/DB/PropertyModifierEnum.h"

EnumPropertyModifier::EnumPropertyModifier
(const CDbObjectDescriptor::TMemberList& memberList
, const CMember* member
, const wxString& label)
{
	const CEnumMember* enumMember = NULL;
	FOR_EACH_CONST(CDbObjectDescriptor::TMemberList, it, memberList)
	{
		//Search for the EnumMember of the same ID
		if ((*it)->GetID() == member->GetID() && *it != member)
		{
			enumMember = static_cast<const CEnumMember*>(*it);
			break;
		}
	}

	if (enumMember)
	{
		wxPGChoices chs;
		FOR_EACH_CONST(CEnumMember::TEnumValues, it, enumMember->GetValues())
		{
			chs.Add(it->m_valueName, it->m_enumValue);
		}

		m_property = new wxEnumProperty(label, GetPropertyName(), chs);
		m_visitor = new CGenericVisitor<U32>();
	}
}

EnumPropertyModifier::~EnumPropertyModifier()
{
	delete m_visitor;
}

void EnumPropertyModifier::ApplyPropertyToMember()
{
	//get property value
	long value = m_property->GetValue().GetInteger();

	//add it to the member
	*(m_visitor->m_value) = static_cast<U32>(value);
}

void EnumPropertyModifier::ApplyMemberToProperty()
{
	//get member value;
	U32 value = *(m_visitor->m_value);

	//add it to the property
	m_property->SetValueFromInt(value);
}

wxPGProperty* EnumPropertyModifier::GetProperty() const { return m_property; }