#pragma once

#include "Game/libs/pugixml/pugixml.hpp"
#include "Game/Core/StringToType.h"
#include "Game/Core/TypeToString.h"
#include "Game/DB/DBManager.h"
#include "Game/Core/Math.h"
#include "Game/Serialization/Member.h"

///////////////////////////////////////////////////////////////////////
//BasicTypeHandler
///////////////////////////////////////////////////////////////////////
template<class MemberType>
class BasicTypeHandler
{
public:
	static void Load( const pugi::xml_node* xmlNode, MemberType& memberType, const char* memberName, CStringID memberID )
	{
		pugi::xml_attribute attribute = xmlNode->attribute( memberName );
		StringUtils::StringToType( attribute.value(), memberType );
	}

	static void Save( pugi::xml_node* xmlNode, MemberType& memberType, const char* memberName, CStringID memberID )
	{
		pugi::xml_attribute xmlAttribute = xmlNode->append_attribute(memberName);
		xmlAttribute.set_value( StringUtils::TypeToString( memberType ).c_str() );
	}
};

///////////////////////////////////////////////////////////////////////
//BasicTypeHandler
///////////////////////////////////////////////////////////////////////
template<>
class BasicTypeHandler<vector3>
{
public:
    static void Load( const pugi::xml_node* xmlNode, vector3& memberType, const char* memberName, CStringID memberID )
    {
        pugi::xml_attribute attribute = xmlNode->attribute( memberName );
        StringUtils::StringToType( attribute.value(), memberType );
    }

    static void Save( pugi::xml_node* xmlNode, vector3& memberType, const char* memberName, CStringID memberID )
    {
        pugi::xml_attribute xmlAttribute = xmlNode->append_attribute(memberName);
        xmlAttribute.set_value( StringUtils::TypeToString( memberType ).c_str() );
    }
};

///////////////////////////////////////////////////////////////////////
//BasicTypeHandler
///////////////////////////////////////////////////////////////////////
template<>
class BasicTypeHandler<CDBFile>
{
public:
    static void Load( const pugi::xml_node* xmlNode, CDBFile& memberType, const char* memberName, CStringID memberID )
    {
        pugi::xml_attribute attribute = xmlNode->attribute( memberName );
        StringUtils::StringToType( attribute.value(), memberType.m_filename );
    }

    static void Save( pugi::xml_node* xmlNode, CDBFile& memberType, const char* memberName, CStringID memberID )
    {
        pugi::xml_attribute xmlAttribute = xmlNode->append_attribute(memberName);
        xmlAttribute.set_value( memberType.m_filename.c_str() );
    }
};



///////////////////////////////////////////////////////////////////////
//BasicTypeHandlerEnum
///////////////////////////////////////////////////////////////////////
template<class MemberType>
class BasicTypeHandlerEnum
{
public:
    static void Load( const pugi::xml_node* xmlNode, MemberType& memberType, const char* memberName, CStringID memberID )
    {
        S32 loadedMember;
        pugi::xml_attribute attribute = xmlNode->attribute( memberName );
        StringUtils::StringToType( attribute.value(), loadedMember ); //string to int
        memberType = static_cast<MemberType>( memberType ); //int to enum
    }

    static void Save( pugi::xml_node* xmlNode, MemberType& memberType, const char* memberName, CStringID memberID )
    {
        pugi::xml_attribute xmlAttribute = xmlNode->append_attribute(memberName);
        S32 saveMember = static_cast<S32>( memberType );//enum to int 
        xmlAttribute.set_value( StringUtils::TypeToString( saveMember ).c_str() );//int to string.
    }
};

///////////////////////////////////////////////////////////////////////
//DBTypeHandler
///////////////////////////////////////////////////////////////////////
template<typename MemberType>
struct DBTypeHandler
{
	static void Load( const pugi::xml_node* xmlNode, MemberType& memberType, const char* memberName, CStringID memberID )
	{
        if( memberName )
        {
            pugi::xml_node xmlChildNode = xmlNode->child( memberName );

            CDbObject* dbObject = &memberType;
            CDbObjectDescriptor& descriptor = dbObject->GetDescriptor();
            descriptor.Load( dbObject, &xmlChildNode );
        }
        else
        {
            CDbObject* dbObject = &memberType;
            CDbObjectDescriptor& descriptor = dbObject->GetDescriptor();
            descriptor.Load( dbObject, xmlNode );
        }
	}

	static void Save( pugi::xml_node* xmlNode, MemberType& memberType, const char* memberName, CStringID memberID )
	{
		pugi::xml_node newXmlNode = xmlNode->append_child( memberName );

		CDbObject* dbObject = &memberType;
		CDbObjectDescriptor& descriptor = dbObject->GetDescriptor();
		descriptor.Save( dbObject, &newXmlNode );
	}
};

///////////////////////////////////////////////////////////////////////
//DBTypeHandler Pointer
///////////////////////////////////////////////////////////////////////
template<typename MemberType>
struct DBTypeHandler<MemberType*>
{
	typedef MemberType* MemberTypePtr;
	static void Load( const pugi::xml_node* xmlNode, MemberTypePtr& memberType, const char* memberName, CStringID memberID )
    {
        if( memberName )
        {
            pugi::xml_node xmlChildNode = xmlNode->child( memberName );
            pugi::xml_attribute attribute = xmlChildNode.attribute( "key" );

            if( attribute.value() != NULL &&  *(attribute.value()) != '\0' )
            {
                CStringID::IDType objectKeyCrc;
                StringUtils::StringToType( attribute.value(), objectKeyCrc );

                memberType = CDBManager::Get()->GetLibraryObjectSafe<MemberType>( objectKeyCrc );
            }
        }
        else
        {
            pugi::xml_attribute attribute = xmlNode->attribute( "key" );

            if( attribute.value() != NULL &&  *(attribute.value()) != '\0' )
            {
                CStringID::IDType objectKeyCrc;
                StringUtils::StringToType( attribute.value(), objectKeyCrc );

                memberType = CDBManager::Get()->GetLibraryObjectSafe<MemberType>( objectKeyCrc );
            }
        }
	}

	static void Save( pugi::xml_node* xmlNode, MemberTypePtr& memberType, const char* memberName, CStringID memberID )
	{
		if( memberType )
		{
			pugi::xml_node newXmlNode = xmlNode->append_child( memberName );
			pugi::xml_attribute newKeyAttribute = newXmlNode.append_attribute( "key" );
			newKeyAttribute.set_value( StringUtils::TypeToString( memberType->GetKey().GetUniqueID() ).c_str() );
		}
	}
};


template<typename MemberType>
class VectorTypeHandler
{
public:
    static void Load( const pugi::xml_node* xmlNode, std::vector<MemberType>& memberType, const char* memberName, CStringID memberID )
    {
        if (xmlNode)
        {
            pugi::xml_node xmlChildNode = xmlNode->child( memberName );

            for (pugi::xml_node xmlCurrentNode = xmlChildNode.first_child(); xmlCurrentNode; xmlCurrentNode = xmlCurrentNode.next_sibling())
            {
                MemberType newMember;
                DBTypeHandler<MemberType>::Load( &xmlCurrentNode, newMember, NULL, memberID );
                memberType.push_back( newMember );
            }
        }
    }

    static void Save( pugi::xml_node* xmlNode, std::vector<MemberType>& memberType, const char* memberName, CStringID memberID )
    {
        pugi::xml_node newXmlNode = xmlNode->append_child( memberName );
        FOR_EACH(std::vector<MemberType> , it, memberType)
        {
            DBTypeHandler<MemberType>::Save( &newXmlNode, *it, "element", memberID );
        }    
    }
};