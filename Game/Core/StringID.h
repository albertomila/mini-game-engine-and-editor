#pragma once

#include "Game/Core/BasicTypes.h"
#include "Game/GameDefs.h"
#include <string>


#if defined(_DEBUG)
	#define STRINGID_USESTRINGCONTENT
#endif

class GAME_DLL CStringID
{
public:
	typedef U32 IDType;

#if !defined(STRINGID_USESTRINGCONTENT)
	typedef CStringID ParamType;
#else //!defined(STRINGID_USESTRINGCONTENT)
	typedef const CStringID& ParamType;
#endif 

	//! Creates an Invalid string ID.
	CStringID();

#if (defined(STRINGID_USESTRINGCONTENT))
	//! Copy constructor (Implement it only if need! Else it prevents register optimization when passed by value in function!)
	CStringID(const CStringID& other);
#endif

	//! Create a string ID out of a string.
	explicit CStringID( const char* str );

	//! Create a string ID out of its CRC, also providing a string (to use for safety checks and debugging)
	explicit CStringID( IDType crc, const char* str );

	//! Create a string ID out of its CRC.
	explicit CStringID( IDType crc );

	//! Returns the string ID's CRC.
	IDType GetUniqueID() const { return m_value; }

	//! Set the content of the string ID post-creation; case-sensitive.
	void SetContent(const char* content);

	//! Returns true if the string ID is valid.
	bool IsValid() const;

	//! Remove the string and set the CRC to the invalid ID
	void Invalidate();
	bool operator==(ParamType r) const;
	bool operator!=(ParamType r) const;
	bool operator<(ParamType r) const;
	bool operator<=(ParamType r) const;
	bool operator>(ParamType r) const;
	bool operator>=(ParamType r) const;


public:
	const static IDType InvalidID = 0xFFFFFFFF;
	const char* c_str() const;	//! Returns the character string value of the string ID.

private:
	void SetContentWithExpectedCRC(const char* content, IDType crc);
	IDType GetCRC32(const char* msg);

	//! The CRC value.
	IDType m_value;

#ifdef STRINGID_USESTRINGCONTENT
private:
	std::string m_content;
#endif //STRINGID_USESTRINGCONTENT
};
