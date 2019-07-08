#pragma once

#include "Game/Core/BasicTypes.h"
#include <string>
#include "Game/Core/Math.h"

namespace StringUtils
{
    inline std::string TypeToString(const vector3& val)
    {
        char str[128];
        sprintf(str, "%g,%g,%g", val[VECT_X], val[VECT_Y], val[VECT_Z]);
        return str;
    }

    inline std::string TypeToString(const vector2& val)
    {
        char str[128];
        sprintf(str, "%g,%g", val[VECT_X], val[VECT_Y] );
        return str;
    }

	inline std::string TypeToString(const std::string& val) // Version used mostly in template methods to stay abstract on the type.
	{
		return val;
	}

    inline std::string TypeToString(bool val)
    {
        return val ? "1" : "0";
    }
    inline std::string TypeToString(char val)
    {
        char str[2];
        sprintf(str, "%c", val);
        return str;
    }

    inline std::string TypeToString( S8 val)
    {
        char str[7];
        sprintf(str, "%i", val);
        return str;
    }
    inline std::string TypeToString(U8 val)
    {
        char str[7];
        sprintf(str, "%u", val);
        return str;
    }
    inline std::string TypeToString( S16 val)
    {
        char str[12];
        sprintf(str, "%i", val);
        return str;
    }
    inline std::string TypeToString( U16 val)
    {
        char str[12];
        sprintf(str, "%u", val);
        return str;
    }

    inline std::string TypeToString( S32 val)
    {
        char str[12];
        sprintf(str, "%li", val);
        return str;
    }
    inline std::string TypeToString( U32 val)
    {
        char str[12];
        sprintf(str, "%lu", val);
        return str;
    }

    inline std::string TypeToString( S64 val)
    {
        char str[23];
        sprintf(str, "%lli", val);

        return str;
    }
    inline std::string TypeToString( U64 val)
    {
        char str[23];

        sprintf(str, "%llu", val);
        return str;
    }
    inline std::string TypeToString( float val)
    {
        char str[32];
        sprintf(str, "%.6f", val);
        return str;
    }
    inline std::string TypeToString( double val)
    {
        char str[32];
        sprintf(str, "%.6f", val);
        return str;
    }

    inline std::string StringIdCrc32TypeToString( const U32 crc )
    {
        char crcValue[32];
//        _snprintf( crcValue, 31, "%%$%08X", crc );
        return crcValue;
    }
}