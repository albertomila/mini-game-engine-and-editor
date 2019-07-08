#pragma once
#include "Game\GameDefs.h"
#include "Game\Core\StringID.h"

class CDynamicObject;

/////////////////////////////////////////////////////////////////////////////////
//CDbObject macro
/////////////////////////////////////////////////////////////////////////////////
#define DECLARE_DYNAMIC_OBJECT( CLASS_TYPE, PARENT_CLASS_TYPE )                    \
public:															                   \
	typedef CLASS_TYPE				self;									       \
	typedef PARENT_CLASS_TYPE		super;		                                   \
	                                                                               \
	static self* StaticCast( CDynamicObject* dynamicObject )                       \
	{                                                                              \
		return static_cast< self* >( dynamicObject );                              \
	}                                                                              \
	                                                                               \
	static self* DynamicCast( CDynamicObject* dynamicObject )                      \
	{                                                                              \
		return dynamic_cast< self* >( dynamicObject );                             \
	}                                                                              \
	static const self* StaticCast( const CDynamicObject* dynamicObject )     \
	{                                                                              \
		return static_cast< const self* >( dynamicObject );                        \
	}                                                                              \
	                                                                               \
	static const self* DynamicCast( const CDynamicObject* dynamicObject )    \
    {                                                                              \
        return dynamic_cast< const self* >( dynamicObject );                       \
	}                                                                              \
	                                                                               \
	                                                                               \
	static const char* GetClassTypeName()                                    \
	{                                                                              \
		return #CLASS_TYPE;                                                        \
	}                                                                              \
                                                                                   \
	static CStringID GetClassType()                                          \
	{                                                                              \
		static const CStringID ms_classType( GetClassTypeName() );				   \
		return ms_classType;													   \
    };                                                                             \
    virtual CStringID GetDerivedClassType()  const                                 \
    {                                                                              \
        return CLASS_TYPE::GetClassType() ;                                         \
    };                                                                              \
	virtual bool IsClassType( CStringID stringID ) const                           \
	{                                                                              \
		if( stringID == self::GetClassType() )                                     \
		{                                                                          \
			return true;                                                           \
		}                                                                          \
		else                                                                       \
		{                                                                          \
			return super::IsClassType( stringID );                                 \
		}                                                                          \
	}                                                                              \
private:																		   \

/////////////////////////////////////////////////////////////////////////////////
//CDbObject macro
/////////////////////////////////////////////////////////////////////////////////
#define DECLARE_ROOT_DYNAMIC_OBJECT( CLASS_TYPE )                                  \
public:															                   \
	typedef CLASS_TYPE				self;									       \
	                                                                               \
	static self* StaticCast( CDynamicObject* dynamicObject )                       \
	{                                                                              \
		return static_cast< self* >( dynamicObject );                              \
	}                                                                              \
	                                                                               \
	static self* DynamicCast( CDynamicObject* dynamicObject )                      \
	{                                                                              \
		return dynamic_cast< self* >( dynamicObject );                             \
	}                                                                              \
	                                                                               \
	static const char* GetClassTypeName()                                          \
	{                                                                              \
		return #CLASS_TYPE;                                                        \
	}                                                                              \
                                                                                   \
	static CStringID GetClassType()                                                \
	{                                                                              \
		static CStringID ms_classType( GetClassTypeName() );                       \
		return ms_classType;													   \
    };                                                                       \
    virtual CStringID GetDerivedClassType()  const                                 \
    {                                                                              \
        return CLASS_TYPE::GetClassType() ;                                         \
    };                                                                                 \
		                                                                               \
	virtual bool IsClassType( CStringID stringID ) const                           \
	{                                                                              \
		return stringID == self::GetClassType();                                   \
	}                                                                              \
private:																		   \
	static const CStringID ms_classType;

/////////////////////////////////////////////////////////////////////////////////
//CDbObject
/////////////////////////////////////////////////////////////////////////////////
class GAME_DLL CDynamicObject
{
	DECLARE_ROOT_DYNAMIC_OBJECT( CDynamicObject )
public:
	CDynamicObject(){}
	virtual ~CDynamicObject(){}
};