#include "Game_PCH.h"
#include "Game/Core/StringID.h"

#include "Game/Core/Crc32.h"

////////////////////////////////////////////////////////////////////////////////
void CStringID::SetContent(const char* content)
{
	if(content == NULL || content[0] == '\0')
	{
		m_value = InvalidID;
#ifdef STRINGID_USESTRINGCONTENT
		m_content.clear();
#endif
	}
	else
	{
		m_value = ComputeCrc32( content );
#ifdef STRINGID_USESTRINGCONTENT
		m_content = content;
#endif
	}
}

////////////////////////////////////////////////////////////////////////////////
void CStringID::SetContentWithExpectedCRC(const char* content, IDType crc)
{
	SetContent(content);

	if (crc != m_value)
	{
		Invalidate();
		//error
	}
}


// Inlines
////////////////////////////////////////////////////////////////////////////////
CStringID::CStringID()
	: m_value(InvalidID)
{
}

CStringID::CStringID(const char* str)
{
	SetContent(str);
}

#if (defined(STRINGID_USESTRINGCONTENT))
////////////////////////////////////////////////////////////////////////////////
CStringID::CStringID(const CStringID& other)
{
	m_value = other.m_value;
#ifdef STRINGID_USESTRINGCONTENT
	m_content = other.m_content;
#endif
}
#endif

////////////////////////////////////////////////////////////////////////////////
CStringID::CStringID(IDType crc)
	: m_value(crc)
{
}

#if defined(STRINGID_USESTRINGCONTENT)
CStringID::CStringID(IDType crc, const char* str)
{
	SetContentWithExpectedCRC(str, crc);
}
////////////////////////////////////////////////////////////////////////////////
#else
CStringID::CStringID(IDType crc, const char*)
	: m_value(crc)
{
}
#endif // #ifdef STRINGID_USESTRINGCONTENT

////////////////////////////////////////////////////////////////////////////////
#ifdef STRINGID_USESTRINGCONTENT
const char* CStringID::c_str() const
{
	return m_content.c_str();
}
#endif // #ifdef STRINGID_USESTRINGCONTENT


//! Returns true if the string ID is valid.
bool CStringID::IsValid() const
{
	return m_value != static_cast<IDType>(InvalidID);
}

//! Remove the string and set the CRC to the invalid ID
void CStringID::Invalidate()
{
	SetContent(NULL);
}

//! Compare two string ID CRCs
bool CStringID::operator==(ParamType r) const
{
	return GetUniqueID() == r.GetUniqueID();
}
bool CStringID::operator!=(ParamType r) const
{
	return GetUniqueID() != r.GetUniqueID();
}

//! Sort operator for storing the string ID in a sorted container.
bool CStringID::operator<(ParamType r) const
{
	return GetUniqueID() < r.GetUniqueID();
}

bool CStringID::operator<=(ParamType r) const
{
	return GetUniqueID() <= r.GetUniqueID();
}

bool CStringID::operator>(ParamType r) const
{
	return GetUniqueID() > r.GetUniqueID();
}

bool CStringID::operator>=(ParamType r) const
{
	return GetUniqueID() >= r.GetUniqueID();
}