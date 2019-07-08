#include "MDGame_PCH.h"
#include "MDGame/Test/DBObjectTest.h"
#include "Game/Serialization/MemberHandler.h"
#include "Game/Serialization/Member.h"

BEGIN_DEFINE_DB_OBJECT( CDBObjectTestNested )
    SERIALIZE_MEMBER( "m7", m_member1, BasicTypeHandler )
    SERIALIZE_MEMBER( "m8", m_member2, BasicTypeHandler )
END_DEFINE_DB_OBJECT()

BEGIN_DEFINE_DB_OBJECT( CDBObjectTest )
	/*SERIALIZE_MEMBER( "m1", m_member1, BasicTypeHandler )
	SERIALIZE_MEMBER( "m2", m_member2, BasicTypeHandler )
	SERIALIZE_MEMBER( "m3", m_member3, BasicTypeHandler )
	SERIALIZE_MEMBER( "m4", m_member4, BasicTypeHandler )
	SERIALIZE_MEMBER( "m5", m_member5, BasicTypeHandler )
    SERIALIZE_MEMBER( "m6", m_member6, BasicTypeHandler )
    SERIALIZE_MEMBER( "MemberBool", m_memberBool, BasicTypeHandler )
    SERIALIZE_MEMBER( "mstring", m_strmember, BasicTypeHandler )*/
    //SERIALIZE_MEMBER( "TextureTest", m_textureTest, BasicTypeHandler )
    
    BEGIN_SERIALIZE_MEMBER_ENUM( "enumMember", m_enumMember )
        REGISTER_ENUM_VALUE( "Invalid", eEnumTest_Invalid )
        REGISTER_ENUM_VALUE( "Enum1", eEnumTest_1 )
        REGISTER_ENUM_VALUE( "Enum2", eEnumTest_2 )
        REGISTER_ENUM_VALUE( "Enum3", eEnumTest_3 )
        REGISTER_ENUM_VALUE( "Enum4", eEnumTest_4 )
        REGISTER_ENUM_VALUE( "MAX", eEnumTest_MAX )
    END_SERIALIZE_MEMBER_ENUM()
    /*
    SERIALIZE_MEMBER( "nestedPtr", m_memberNestedPtr, DBTypeHandler )

    BEGIN_GROUP( "GroupTest" )
        SERIALIZE_MEMBER( "memberNested3", m_memberNested3, BasicTypeHandler )
        SERIALIZE_MEMBER( "nested1", m_memberNested1, DBTypeHandler )
        SERIALIZE_MEMBER( "nested2", m_memberNested2, DBTypeHandler )
    END_GROUP()


    SERIALIZE_MEMBER( "memberVector", m_memberVector, VectorTypeHandler )*/
    //SERIALIZE_MEMBER( "memberVectorPtr", m_memberVectorPtr, VectorTypeHandler )
    SERIALIZE_MEMBER( "pos", m_position, BasicTypeHandler )
    SERIALIZE_MEMBER( "pos2", m_position2, BasicTypeHandler )
    
END_DEFINE_DB_OBJECT()


CDBObjectTestNested::CDBObjectTestNested()
	: m_member1(4)
	, m_member2(2)
{
}

CDBObjectTestNested::~CDBObjectTestNested() {}


CDBObjectTest::CDBObjectTest()
	: m_member1(1)
	, m_member2(2)
	, m_member3(3)
	, m_member4(4)
	, m_member5(5.0f)
	, m_member6(6.0)
	, m_memberNested3(99.5f)
	, m_memberNestedPtr(NULL)
	, m_enumMember(eEnumTest_4)
{
	m_strmember = std::string("teststring");
}

CDBObjectTest::~CDBObjectTest() {}