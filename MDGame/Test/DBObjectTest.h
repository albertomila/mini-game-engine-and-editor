#pragma once
#include "Game/DB/DBObject.h"
#include "MDGame/MDGameDefs.h"
#include "Game/Core/Math.h"
#include <string>

class MDGAME_DLL CDBObjectTestNested : public CDbObject
{
	DECLARE_DB_OBJECT( CDBObjectTestNested, CDbObject );

public:
	CDBObjectTestNested();

	virtual ~CDBObjectTestNested();

private:
    U8 m_member1;
    U8 m_member2;
};

class MDGAME_DLL CDBObjectTest : public CDbObject
{
	DECLARE_DB_OBJECT( CDBObjectTest, CDbObject );

public:
    enum EEnumTest
    {
        eEnumTest_Invalid = -1
        , eEnumTest_1
        , eEnumTest_2
        , eEnumTest_3
        , eEnumTest_4
        , eEnumTest_MAX
    };

	CDBObjectTest();

	virtual ~CDBObjectTest();

private:
	U8 m_member1;
	U16 m_member2;
	U32 m_member3;
	U64 m_member4;
	float m_member5;
	double m_member6;
    std::string m_strmember;
    EEnumTest m_enumMember;
    bool        m_memberBool;
    vector3       m_position;
    vector2       m_position2;
    CDBFile     m_textureTest;

    float m_memberNested3;
    CDBObjectTestNested m_memberNested1;
    CDBObjectTestNested m_memberNested2;
	CDBObjectTest* m_memberNestedPtr;

    std::vector<CDBObjectTestNested>  m_memberVector;
    std::vector<CDBObjectTestNested*>  m_memberVectorPtr;
};