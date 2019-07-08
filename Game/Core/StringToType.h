#pragma once
#include <string>
#include "Game/Core/Math.h"
namespace StringUtils
{
    inline bool StringToType(const char* str, vector3& dest)
    {
        return sscanf(str, "%g,%g,%g", &dest[VECT_X], &dest[VECT_Y], &dest[VECT_Z]) == 3;
    }
    inline bool StringToType(const char* str, vector2& dest)
    {
        return sscanf(str, "%g,%g", &dest[VECT_X], &dest[VECT_Y] ) == 3;
    }
	inline bool StringToType(const char* str, std::string& dest)
	{
		dest = std::string( str );
		return true;
	}

    inline bool StringToType(const char* str, bool& val)
    {
        if ((str[0] == '0' || str[0] == '1') && (str[1] == '\0' || str[1] == ' ' || str[1] == ',' || str[1] == ';'))
        {
            val = *str == '1';
            return true;
        }
        return false;
    }
    inline bool StringToType(const char* str, char& val)
    {
        return sscanf(str, "%c", &val) == 1;
    }
    inline bool StringToType(const char* str, S8& val)
    {
        int i;
        if( sscanf(str, "%i", &i) == 1 )
        {
            val = i;
            return true;
        }
        return false;
    }
    inline bool StringToType(const char* str, U8& val)
    {
        unsigned int u;
        if( sscanf(str, "%u", &u) == 1 )
        {
            val = u;
            return true;
        }
        return false;
    }
    inline bool StringToType(const char* str, S16& val)
    {
        return sscanf(str, "%i", &val) == 1;
    }
    inline bool StringToType(const char* str, U16& val)
    {
        return sscanf(str, "%u", &val) == 1;
    }

    inline bool StringToType(const char* str, S32& val)
    {
        return sscanf(str, "%li", &val) == 1;
    }
    inline bool StringToType(const char* str, U32& val)
    {
        return sscanf(str, "%lu", &val) == 1;
    }
    inline bool StringToType(const char* str, S64& val)
    {
            return sscanf(str, "%lli", &val) == 1;
    }
    inline bool StringToType(const char* str, U64& val)
    {
        return sscanf(str, "%llu", &val) == 1;
    }
    inline bool StringToType(const char* str, float& val)
    {
        return sscanf(str, "%g", &val) == 1;
    }

	inline bool StringToType(const char* str, double& val)
	{
		return sscanf(str, "%lg", &val) == 1;
	}
}
