#include "Editor_PCH.h"
#include "Editor/DB/PropertyModifierHelpers.h"

U32 SPropertyIDHelpers::ms_nextPropertyID = 0;

////////////////////////////////////////////////////////////////////////////
//SPropertyIDHelpers
////////////////////////////////////////////////////////////////////////////
/*static*/ U32 SPropertyIDHelpers::GetNextPropertyID()
{
	return ++ms_nextPropertyID;
}

/*static*/ std::string SPropertyIDHelpers::GetPropertyName(U32 propertyID)
{
	std::string propertyLabel("Property_");
	propertyLabel += StringUtils::TypeToString(propertyID);
	return propertyLabel;
}

/*static*/ std::string SPropertyIDHelpers::GetNextPropertyName()
{
	return GetPropertyName(GetNextPropertyID());
}

////////////////////////////////////////////////////////////////////////////
//IPropertyModifier
////////////////////////////////////////////////////////////////////////////
IPropertyModifier::IPropertyModifier()
	: m_propertyID(SPropertyIDHelpers::GetNextPropertyID())
{
}

IPropertyModifier::~IPropertyModifier() {}

std::string IPropertyModifier::GetPropertyName() const
{
	std::string propertyLabel("Property_");
	propertyLabel += StringUtils::TypeToString(m_propertyID);
	return propertyLabel;
}
