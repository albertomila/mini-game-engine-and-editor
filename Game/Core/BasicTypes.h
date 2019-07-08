#pragma once
#include <string>
#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;
typedef uint64_t ui64;

typedef signed __int8		S8;
typedef signed __int16		S16;
typedef signed __int32		S32;
typedef signed __int64		S64;

typedef unsigned __int8		U8;
typedef unsigned __int16	U16;
typedef unsigned __int32	U32;
typedef unsigned __int64	U64;

const S8  S8_Min                    = -128;                       
const S8  S8_Max                    = 127;                        
const U8  U8_Min                    = 0;                          
const U8  U8_Max                    = 255;                        
const S16 S16_Min                   = -32768;                     
const S16 S16_Max                   = 32767;                      
const U16 U16_Min                   = 0;                          
const U16 U16_Max                   = 65535;                      
const S32 S32_Min                   = 0x80000000;                 
const S32 S32_Max                   = 0x7FFFFFFF;                 
const U32 U32_Min                   = 0x00000000;                 
const U32 U32_Max                   = 0xFFFFFFFF;                 
const S64 S64_Min                   = 0x8000000000000000LL;       
const S64 S64_Max                   = 0x7FFFFFFFFFFFFFFFLL;       
const U64 U64_Min                   = 0x0000000000000000ULL;      
const U64 U64_Max                   = 0xFFFFFFFFFFFFFFFFULL;  

struct CDBFile
{
    CDBFile(){}
    CDBFile( const std::string& filename) : m_filename(filename) {}
    std::string m_filename;
};

template<class T>
inline void Unused( const T& value )
{
};

#define DEBUG_END_SCOPE Unused( 0 );

#define STRINGIZE( NAME ) #NAME