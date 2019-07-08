#pragma once
#include "Game\Core\Singleton.h"
#include "Game\Core\StringID.h"
#include "Game\DB\DBObject.h"
#include "MDGame\MDGameDefs.h"
#include <unordered_map>

class GAME_DLL CDBManager : public CSingleton<CDBManager>
{
public:
	typedef CDbObject* (*TObjectCreator)();

	typedef std::unordered_map< CStringID::IDType, CDbObject* >	TSingleObjectMap;
	typedef TSingleObjectMap::iterator				TObjMapIt;
	typedef TSingleObjectMap::const_iterator		TObjMapConstIt;

	typedef std::unordered_map< CStringID::IDType, CDbObject* >	TLibraryMap;
	typedef TLibraryMap::iterator					TLibMapIt;
	typedef TLibraryMap::const_iterator				TLibMapConstIt;

	typedef std::unordered_map< CStringID::IDType, TLibraryMap > TTempLibraryObjectsMap;
	typedef TTempLibraryObjectsMap::iterator				TTempObjMapIt;
	typedef TTempLibraryObjectsMap::const_iterator			TTempObjMapConstIt;
	
	struct SDBObjectType 
	{
		TObjectCreator		m_creator;
		TSingleObjectMap	m_singleObjects;
		TLibraryMap			m_libraryObjects;
		const char*			m_libPrefix;
	};

	typedef  std::unordered_map< CStringID::IDType, SDBObjectType > TDBMap;
	typedef  TDBMap::iterator						   TDBMapIt;
	typedef  TDBMap::const_iterator					   TDBMapConstIt;

public:
	CDBManager(){}

	template<class T>
	void RegisterSingleObject( const char* objectIDName );

	template<class T>
	void RegisterSingleObject( const CStringID objectID, const char* objectIDName );

	template<class T>
	const T* GetSingleObject( const CStringID objectID ) const;
	
	//Npn-const version, to remove, only for test purposes
	template<class T>
	T* GetSingleObject( const CStringID objectID );

	template<class T>
	void RegisterSingleLibrary( const char* libraryID );

	template<class T>
	void RegisterSingleLibraryObject( const char* objectIDName );

	template<class T>
	void RegisterSingleLibraryObject( CStringID objectID, const char* objectIDName );

	template<class T>
	T* GetLibraryObjectSafe( U32 objectKey );

	template<class T>
	const TLibraryMap* GetLibrary() const;

	const TLibraryMap* GetLibrary(CStringID libraryID) const;

    template<class T>
	const char* GetLibraryPrefix() const;

	const char* GetLibraryPrefix(CStringID libraryID) const;

	void SaveAllDb();
	void LoadAllDb();

    const TDBMap& GetAllData() const{ return m_dbMap; }
private:
	TTempLibraryObjectsMap	m_tempObjects;
	TDBMap					m_dbMap;
};

template<class T>
void CDBManager::RegisterSingleObject(const char* objectIDName)
{
	RegisterSingleObject<T>(CStringID(objectIDName), objectIDName);
}

template<class T>
void CDBManager::RegisterSingleObject(const CStringID objectID, const char* objectIDName)
{
	CStringID classTypeID = T::GetClassType();

	TDBMapIt it = m_dbMap.find(classTypeID.GetUniqueID());
	if (it == m_dbMap.end())
	{
		T::RegisterProperties();
	}

	SDBObjectType& objTypeContainer = m_dbMap[classTypeID.GetUniqueID()];
	const TSingleObjectMap& objectMap = objTypeContainer.m_singleObjects;
	TObjMapConstIt itObj = objectMap.find(objectID.GetUniqueID());
	if (itObj != objectMap.end())
	{
		//error
		return;
	}

	objTypeContainer.m_creator = T::CreateObject;
	CDbObject* newObject = objTypeContainer.m_creator();
	newObject->SetKey(objectID);
	newObject->SetName(objectIDName);
	objTypeContainer.m_singleObjects[objectID.GetUniqueID()] = newObject;
}

template<class T>
const T* CDBManager::GetSingleObject(const CStringID objectID) const
{
	TDBMapConstIt it = m_dbMap.find(T::GetClassType().GetUniqueID());
	if (it != m_dbMap.end())
	{
		const TSingleObjectMap& objectMap = it->second.m_singleObjects;

		TObjMapConstIt itObj = objectMap.find(objectID.GetUniqueID());
		if (itObj != objectMap.end())
		{
			//second is CDbObject*
			return T::DynamicCast(itObj->second);
		}
	}

	return NULL;
}

//Npn-const version, to remove, only for test purposes
template<class T>
T* CDBManager::GetSingleObject(const CStringID objectID)
{
	TDBMapIt it = m_dbMap.find(T::GetClassType().GetUniqueID());
	if (it != m_dbMap.end())
	{
		TSingleObjectMap& objectMap = it->second.m_singleObjects;

		TObjMapIt itObj = objectMap.find(objectID.GetUniqueID());
		if (itObj != objectMap.end())
		{
			//second is CDbObject*
			return T::DynamicCast(itObj->second);
		}
	}

	return NULL;
}

template<class T>
void CDBManager::RegisterSingleLibrary(const char* libraryID)
{
	CStringID classTypeID = T::GetClassType();

	TDBMapIt it = m_dbMap.find(classTypeID.GetUniqueID());
	if (it == m_dbMap.end())
	{
		T::RegisterProperties();
	}

	SDBObjectType& objTypeContainer = m_dbMap[classTypeID.GetUniqueID()];
	objTypeContainer.m_creator = T::CreateObject;
	objTypeContainer.m_libPrefix = libraryID;
}

template<class T>
void CDBManager::RegisterSingleLibraryObject(const char* objectIDName)
{
	RegisterSingleLibraryObject<T>(CStringID(objectIDName), objectIDName);
}

template<class T>
void CDBManager::RegisterSingleLibraryObject(CStringID objectID, const char* objectIDName)
{
	CStringID classTypeID = T::GetClassType();
	TDBMapIt it = m_dbMap.find(classTypeID.GetUniqueID());
	if (it != m_dbMap.end())
	{
		SDBObjectType& objTypeContainer = it->second;
		TLibraryMap& libMap = objTypeContainer.m_libraryObjects;

		TObjMapConstIt itObj = libMap.find(objectID.GetUniqueID());
		if (itObj == libMap.end())
		{
			CDbObject* newLibraryObject = objTypeContainer.m_creator();
			newLibraryObject->SetKey(objectID);
			newLibraryObject->SetName(objectIDName);
			libMap[objectID.GetUniqueID()] = newLibraryObject;
		}
		else
		{
			//error
		}
	}
	else
	{
		//error
	}
}

template<class T>
T* CDBManager::GetLibraryObjectSafe(U32 objectKey)
{
	CDbObject* dbObject = NULL;
	CStringID classTypeID = T::GetClassType();

	//search on the main db
	TDBMapIt it = m_dbMap.find(classTypeID.GetUniqueID());
	if (it != m_dbMap.end())
	{
		SDBObjectType& objTypeContainer = it->second;
		TLibraryMap& libMap = objTypeContainer.m_libraryObjects;

		TObjMapIt itObj = libMap.find(objectKey);
		if (itObj != libMap.end())
		{
			dbObject = itObj->second;
		}
	}

	if (!dbObject)
	{
		//search on the temporary db
		TLibraryMap& libMap = m_tempObjects[classTypeID.GetUniqueID()];
		TObjMapIt itObj = libMap.find(objectKey);
		if (itObj != libMap.end())
		{
			dbObject = itObj->second;
		}
		else
		{
			//create a temp object if it doesn't exist yet
			dbObject = T::CreateObject();
			dbObject->SetKey(CStringID(objectKey));
			libMap[objectKey] = dbObject;
		}
	}

	//assert
	if (dbObject)
	{
		return T::DynamicCast(dbObject);
	}

	return NULL;
}

template<class T>
const CDBManager::TLibraryMap* CDBManager::GetLibrary() const
{
	return GetLibrary(T::GetClassType());
}