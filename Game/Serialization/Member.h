#pragma once
#include "Game\Core\StringID.h"
#include <typeinfo>       // operator typeid
#include <iostream>
#include <ostream>
#include "Game/libs/pugixml/pugixml.hpp"
#include <vector>
#include "Game/Core/Callback.h"
#include "Game/Core/Algorithm.h"
#include "Game/Core/Math.h"
/*
//forward declarations
namespace pugi
{
	class xml_node;
}
*/
class CDbObject;

class GAME_DLL ITypeVisitor
{
protected:
	virtual ~ITypeVisitor();
public:
    virtual void Visit( void* value ) = 0;
};

class GAME_DLL ITypeVisitorPtr : public ITypeVisitor
{
protected:
	virtual ~ITypeVisitorPtr();
public:
    virtual void Visit( void* value ) = 0;
    CStringID m_libraryID;
};

class GAME_DLL CDBObjectVectorVisitor : public ITypeVisitor
{
public:
	CDBObjectVectorVisitor();
    virtual ~CDBObjectVectorVisitor();
	virtual void Visit(void* value) override;

    const CDbObject* m_parent;
    std::vector<CDbObject*> m_dbObjectList;
    CCallback< CDBObjectVectorVisitor* > m_onDataChangedCallback;
    CCallback< CDBObjectVectorVisitor* > m_onAddNewEntryCallback;
};


struct MemberTypeIDEnum{};
struct MemberTypeIDGroup{};
struct MemberTypeDBObject{};
struct MemberTypeDBObjectPtr{};
struct MemberTypeVector{};

//DBObject
template< class T >
struct MemberToTypeID
{ 
    static const CStringID::IDType value = 0x05A27857;
};
template<> struct MemberToTypeID<MemberTypeDBObject>{ static const CStringID::IDType value = 0x05A27857; /*"DBObject" crc*/  };

//DBObject*
template< class T >
struct MemberToTypeID<T*>
{ 
    static const CStringID::IDType value = 0xD068524A;
};
template<> struct MemberToTypeID<MemberTypeDBObjectPtr>{ static const CStringID::IDType value = 0xD068524A; /*"DBObject*" crc*/  };

//DBObject*
template< class T >
struct MemberToTypeID<std::vector<T>>
{ 
    static const CStringID::IDType value = 0x40D82C05;
};
template<> struct MemberToTypeID<MemberTypeVector>{ static const CStringID::IDType value = 0x40D82C05; /*"std::vector*" crc*/  };

template<> struct MemberToTypeID<U8>{  static const CStringID::IDType value = 0xAE170370; /*U8 crc*/  };
template<> struct MemberToTypeID<U16>{ static const CStringID::IDType value = 0x9CD1A762; /*U8 crc*/  };
template<> struct MemberToTypeID<U32>{ static const CStringID::IDType value = 0xA98A01F9; /*U8 crc*/  };
template<> struct MemberToTypeID<U64>{ static const CStringID::IDType value = 0x3D9E5089; /*U8 crc*/  };

template<> struct MemberToTypeID<S8>{  static const CStringID::IDType value = 0xF84DA4F6; /*U8 crc*/  };
template<> struct MemberToTypeID<S16>{ static const CStringID::IDType value = 0x985CDBD0; /*U8 crc*/  };
template<> struct MemberToTypeID<S32>{ static const CStringID::IDType value = 0xAD077D4B; /*U8 crc*/  };
template<> struct MemberToTypeID<S64>{ static const CStringID::IDType value = 0x39132C3B; /*U8 crc*/  };

template<> struct MemberToTypeID<bool>{ static const CStringID::IDType value = 0x55813692; /*U8 crc*/  };
template<> struct MemberToTypeID<float>{  static const CStringID::IDType value = 0xC9A55E95; /*U8 crc*/  };
template<> struct MemberToTypeID<double>{ static const CStringID::IDType value = 0xDAE7F2EF; /*U8 crc*/  };

template<> struct MemberToTypeID<std::string>{ static const CStringID::IDType value = 0xC508D6F7; /*U8 crc*/  };

template<> struct MemberToTypeID<MemberTypeIDEnum>{ static const CStringID::IDType value = 0x6ACCBEE3; /*U8 crc*/  };
template<> struct MemberToTypeID<MemberTypeIDGroup>{ static const CStringID::IDType value = 0x48CBE475; /*U8 crc*/  };

template<> struct MemberToTypeID<CDBFile>{ static const CStringID::IDType value = 0x539CB5FB; /*U8 crc*/  };

template<> struct MemberToTypeID<vector3>{ static const CStringID::IDType value = 0xC39B62DA; /* "vector3" crc*/  };
template<> struct MemberToTypeID<vector2>{ static const CStringID::IDType value = 0xB49C524C; /* "vector2" crc*/  };


///////////////////////////////////////////////////////////////////////
//CMember
///////////////////////////////////////////////////////////////////////
class GAME_DLL CMember
{
public:
	CMember(const char* memberName);
	
	virtual void Load( CDbObject* parent, const pugi::xml_node* xmlNode ) const = 0;
	virtual void Save( const CDbObject* parent, pugi::xml_node* xmlNode ) const = 0; 
	virtual void Visit( const CDbObject* parent, ITypeVisitor* visitor ) = 0; 

	const char* GetName() const{ return m_memberName; }
	CStringID GetID() const{ return m_memberID; }

    virtual CStringID::IDType GetMemberTypeID() const = 0;
protected:
	const char* m_memberName;
	CStringID m_memberID;
};

///////////////////////////////////////////////////////////////////////
//CGenericMember
///////////////////////////////////////////////////////////////////////
template<class ClassType, class MemberType, class MemberHandler>
class CGenericMember : public CMember
{
public:
	CGenericMember( const char* memberName, U32 memberOffset )
		: CMember( memberName )
		, m_memberOffset( memberOffset )
	{

	}
	
	MemberType& GetMember(const CDbObject* parent) const
	{
		const MemberType* r = reinterpret_cast<const MemberType*>( (reinterpret_cast<const char*>(parent) + m_memberOffset) );
		return *const_cast<MemberType*>(r);
	}
	
	virtual void Load( CDbObject* parent, const pugi::xml_node* xmlNode ) const override
	{
		MemberHandler::Load( xmlNode, GetMember(parent), GetName(), GetID() );
	}

	virtual void Save( const CDbObject* parent, pugi::xml_node* xmlNode) const override
	{
		MemberHandler::Save( xmlNode, GetMember(parent), GetName(), GetID() );
    }

    virtual void Visit( const CDbObject* parent, ITypeVisitor* visitor ) override
    {
        MemberType& memberType = GetMember(parent);
        visitor->Visit( &memberType );
    }

    virtual CStringID::IDType GetMemberTypeID() const
    {
        return MemberToTypeID<MemberType>::value;
    }

    void SetLibraryID( CStringID libraryID ){ m_libraryID = libraryID; }
    CStringID GetLibraryID(){ return m_libraryID; }

private:
    U32 m_memberOffset;
    CStringID m_libraryID;
};

///////////////////////////////////////////////////////////////////////
//CGenericMember
///////////////////////////////////////////////////////////////////////
template<class ClassType, class MemberType, class MemberHandler>
class CGenericMemberPtr : public CGenericMember<ClassType,MemberType,MemberHandler>
{
public:
    CGenericMemberPtr( const char* memberName, U32 memberOffset )
        : CGenericMember( memberName, memberOffset)
    {

    }

    virtual void Visit( const CDbObject* parent, ITypeVisitor* visitor ) override
    {
        MemberType& memberType = GetMember(parent);

        ITypeVisitorPtr* typeVisitorPtr = dynamic_cast<ITypeVisitorPtr*>( visitor );
        typeVisitorPtr->m_libraryID = m_libraryID;
        typeVisitorPtr->Visit( &memberType );
    }

    void SetLibraryID( CStringID libraryID ){ m_libraryID = libraryID; }
    CStringID GetLibraryID(){ return m_libraryID; }

private:
    CStringID m_libraryID;
};



template<class ClassType, class MemberType, class MemberHandler>
class CGenericEnumMember : public CGenericMember<ClassType, MemberType, MemberHandler>
{
public:
    CGenericEnumMember( const char* memberName, U32 memberOffset )
        : CGenericMember( memberName, memberOffset )
    {
    }

    virtual CStringID::IDType GetMemberTypeID() const
    {
        return MemberToTypeID<MemberTypeIDEnum>::value;
    }
};

///////////////////////////////////////////////////////////////////////
//CGenericMember
///////////////////////////////////////////////////////////////////////
template<class ClassType, class MemberType, class MemberHandler>
class CGenericMember<ClassType, std::vector<MemberType>, MemberHandler> : public CMember
{
public:
    typedef std::vector<MemberType> TMemberType;
    typedef CGenericMember<ClassType, std::vector<MemberType>, MemberHandler> self;

    CGenericMember( const char* memberName, U32 memberOffset )
        : CMember( memberName )
        , m_memberOffset( memberOffset )
    {

    }

    TMemberType& GetMember(const CDbObject* parent) const
    {
        const TMemberType* r = reinterpret_cast<const TMemberType*>( (reinterpret_cast<const char*>(parent) + m_memberOffset) );
        return *const_cast<TMemberType*>(r);
    }

    virtual void Load( CDbObject* parent, const pugi::xml_node* xmlNode ) const override
    {
        VectorTypeHandler<MemberType>::Load( xmlNode, GetMember(parent), GetName(), GetID() );
    }

    virtual void Save( const CDbObject* parent, pugi::xml_node* xmlNode) const override
    {
        VectorTypeHandler<MemberType>::Save( xmlNode, GetMember(parent), GetName(), GetID() );
    }

    virtual void Visit( const CDbObject* parent, ITypeVisitor* visitor ) override
    {
        TMemberType& memberType = GetMember(parent);
        std::vector< CDbObject* > dbObjectList;
        FOR_EACH( std::vector<MemberType>, it, memberType )
        {
            CDbObject* dbObject = &(*it);
            dbObjectList.push_back( dbObject );
        }

        CDBObjectVectorVisitor* vectorVisitor = static_cast<CDBObjectVectorVisitor*>( visitor );
        vectorVisitor->m_onDataChangedCallback = CCallback< CDBObjectVectorVisitor* >( this,  &self::DataEditedCallback );
        vectorVisitor->m_onAddNewEntryCallback = CCallback< CDBObjectVectorVisitor* >( this,  &self::AddNewEntryCallback );
        vectorVisitor->m_parent = parent;

        vectorVisitor->Visit( &dbObjectList );
    }

    void AddNewEntryCallback( CDBObjectVectorVisitor* visitor )
    {
        CDbObject* newDBObject = MemberType::CreateObject();
        visitor->m_dbObjectList.push_back( newDBObject );
    }

    void DataEditedCallback( CDBObjectVectorVisitor* visitor )
    {
        TMemberType& memberType = GetMember(visitor->m_parent);
        memberType.resize( 0 );
        FOR_EACH( std::vector<CDbObject*>, it, visitor->m_dbObjectList )
        {
            CDbObject* dbObject = *it;
            MemberType* newElementPtr = static_cast< MemberType* >( dbObject );
            if( newElementPtr )
            {
                MemberType newElement = *newElementPtr;
                memberType.push_back( newElement );
            } 
        }
    }

    CCallback< std::vector<CDbObject*> > m_onAddNewEntryCallback;
    virtual CStringID::IDType GetMemberTypeID() const
    {
        return MemberToTypeID<MemberTypeVector>::value;
    }

private:
    U32 m_memberOffset;
    MemberType m_tempMember;
};


///////////////////////////////////////////////////////////////////////
//CEnumMember
///////////////////////////////////////////////////////////////////////
class GAME_DLL CEnumMember : public CMember
{
public:
    struct SEnumValue
    {
		SEnumValue(const char* valueName, S32 enumValue);

        const char* m_valueName;
        S32 m_enumValue;
    };
    typedef std::vector<SEnumValue> TEnumValues;

	CEnumMember(const char* enumName);
    virtual void Load( CDbObject* parent, const pugi::xml_node* xmlNode ) const override;
    virtual void Save( const CDbObject* parent, pugi::xml_node* xmlNode) const override;
    const std::vector<SEnumValue>& GetValues() const { return m_values; }
    void PushEnumValue( const char* valueName, U32 enumValue );
    virtual void Visit( const CDbObject* parent, ITypeVisitor* visitor ) override;
	virtual CStringID::IDType GetMemberTypeID() const;

private:
    std::vector<SEnumValue> m_values;
};

///////////////////////////////////////////////////////////////////////
//CGroupMember
///////////////////////////////////////////////////////////////////////
class CGroupMember : public CMember
{
public:
    typedef std::vector< CMember* > TMemberList;

    CGroupMember(const char* groupName);
    virtual void Load( CDbObject* parent, const pugi::xml_node* xmlNode ) const override;
    virtual void Save( const CDbObject* parent, pugi::xml_node* xmlNode) const override;
    virtual void Visit( const CDbObject* parent, ITypeVisitor* visitor ) override;
    virtual CMember* GetChildMember( const CStringID& name, const CDbObject* parent ) const;
	virtual CStringID::IDType GetMemberTypeID() const;

    TMemberList& GetMemberList() { return ms_members; }
    const TMemberList& GetMemberList() const{ return ms_members; }

private:
    TMemberList ms_members;
};
///////////////////////////////////////////////////////////////////////
//MemberCreator
///////////////////////////////////////////////////////////////////////
namespace MemberCreator
{
	template<class ObjectType, class MemberType> 
	U32 GetOffsetMember(MemberType ObjectType::* memberPtr)
	{
		const uintptr_t fakeNonNullAddress = 32; // could be any non zero value; conversion of zero valued C++ pointers always yield zero instead of member delta 

		// compute offset of CNomadObject inside ObjectType
		void * objectPointer = reinterpret_cast<ObjectType *>(fakeNonNullAddress);
		uintptr_t NomadObjectOffset = reinterpret_cast<uintptr_t>(objectPointer) - fakeNonNullAddress;

		// compute offset of MemberType inside ObjectType
		uintptr_t memberOffset = reinterpret_cast<uintptr_t>(&(reinterpret_cast<ObjectType *>(fakeNonNullAddress)->*memberPtr)) - fakeNonNullAddress;

		// compute offset of MemberType inside CNomadObject part of ObjectType
		uintptr_t CorrectedMemberOffset = memberOffset - NomadObjectOffset;

		return U32(CorrectedMemberOffset);
	}

    template< template <class T> 
              class MemberHandler
            , class ClassType
            , class MemberType>
        CMember* CreateEnumMember( const char* memberName, MemberType ClassType::* memberPtr )
    {
        U32 offset = GetOffsetMember( memberPtr );
        CMember* newMember = new CGenericEnumMember<ClassType, MemberType, MemberHandler<MemberType> >( memberName, offset );
        return newMember;
    }
	
	template< class MemberHandler
			, class ClassType
			, class MemberType>
	CMember* Create( const char* memberName, MemberType ClassType::* memberPtr )
	{
		U32 offset = GetOffsetMember( memberPtr );
		CMember* newMember = new CGenericMember<ClassType, MemberType, MemberHandler>( memberName, offset );
		return newMember;
	}

	template< template <class T> 
			  class MemberHandler
			, class ClassType
			, class MemberType>
	CMember* Create( const char* memberName, MemberType ClassType::* memberPtr )
	{
		return Create<MemberHandler<MemberType>, ClassType, MemberType>( memberName, memberPtr );
    }

    template< template <class T> 
    class MemberHandler
        , class ClassType
        , class MemberType>
        CMember* Create( const char* memberName, MemberType* ClassType::* memberPtr )
    {
        typedef CGenericMemberPtr<ClassType, MemberType*, MemberHandler<MemberType*>> TMember;

        U32 offset = GetOffsetMember( memberPtr );
        TMember* newMember = new TMember( memberName, offset );
        newMember->SetLibraryID( CStringID( MemberType::GetClassTypeName() ));
        return newMember;
    }

    template< template <class T> class MemberHandler
            , class ClassType
            , class MemberType>
    CMember* Create( const char* memberName, std::vector<MemberType> ClassType::* memberPtr )
    {
        return Create<MemberHandler< std::vector<MemberType> >, ClassType, std::vector<MemberType> >( memberName, memberPtr );
    }
}