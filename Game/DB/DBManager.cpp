#include "Game_PCH.h"
#include "Game/DB/DBManager.h"
#include "Game/libs/pugixml/pugixml.hpp"
#include "Game/Core/StringToType.h"
#include "Game/Core/Path.h"

////////////////////////////////////////////////////////////////////////////////
void CDBManager::SaveAllDb()
{
	//load all single objects
	FOR_EACH( TDBMap, it, m_dbMap )
	{
		SDBObjectType& dbType = it->second;

		TSingleObjectMap& objectMap = dbType.m_singleObjects;
		FOR_EACH( TSingleObjectMap, itSingleObject, objectMap )
		{
			CDbObject* dbObject = itSingleObject->second;

			pugi::xml_document doc;
			pugi::xml_node rootNode = doc.append_child();
			rootNode.set_name("SingleObject");
			rootNode.append_attribute("name") = dbObject->GetName();

			dbObject->GetDescriptor().Save( dbObject, &rootNode );

			std::string filename;
			PathHelpers::GetDataBasePath(filename);
			filename.append( dbObject->GetName() );
			filename.append( ".xml");
			doc.save_file( filename.c_str() );
		}

		TLibraryMap& libMap = dbType.m_libraryObjects;
		if( !libMap.empty() )
		{
			pugi::xml_document doc;
			pugi::xml_node rootNode = doc.append_child();
			rootNode.set_name("Library");
			rootNode.append_attribute("name") = dbType.m_libPrefix;

			FOR_EACH( TLibraryMap, itLibrary, libMap )
			{
				CDbObject* dbObject = itLibrary->second;

				pugi::xml_node libraryObjectNode = rootNode.append_child();
				libraryObjectNode.set_name("LibraryObject");
				libraryObjectNode.append_attribute("name") = dbObject->GetName();
				libraryObjectNode.append_attribute("key") = dbObject->GetKey().GetUniqueID();

				dbObject->GetDescriptor().Save( dbObject, &libraryObjectNode );
			}

			std::string filename;
			PathHelpers::GetDataBasePath(filename);
			filename.append( dbType.m_libPrefix );
			filename.append( ".xml");
			doc.save_file( filename.c_str() );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
void CDBManager::LoadAllDb()
{
	//load all db
	FOR_EACH( TDBMap, it, m_dbMap )
	{
		SDBObjectType& dbType = it->second;

		//load all libraries
		{
			TLibraryMap& libMap = dbType.m_libraryObjects;

			std::string filename;
			PathHelpers::GetDataBasePath(filename);
			filename.append( dbType.m_libPrefix );
			filename.append( ".xml");
			pugi::xml_document doc;
			if ( doc.load_file( filename.c_str() ))
            {
                pugi::xml_node xmlRootNode = doc.child( "Library" );

                if (xmlRootNode)
                {
                    for (pugi::xml_node_iterator it = xmlRootNode.begin(); it != xmlRootNode.end(); ++it)
                    {
                        const pugi::xml_node& xmlCurrentNode = *it;

                        CDbObject* newObject = dbType.m_creator();

                        pugi::xml_attribute xmlAttribute = xmlCurrentNode.attribute( "name" );
                        const char* libraryObjectName = xmlAttribute.value();
                        CStringID libraryObjectID( libraryObjectName );

                        newObject->SetKey( libraryObjectID );
                        newObject->SetName( libraryObjectName );
                        libMap[ libraryObjectID.GetUniqueID() ] = newObject;

                        newObject->GetDescriptor().Load( newObject, &xmlCurrentNode );
                    }
                }
			}

		}

		//load all single objects
		{
			TSingleObjectMap& objectMap = dbType.m_singleObjects;
			FOR_EACH( TSingleObjectMap, itSingleObject, objectMap )
			{
				CDbObject* dbObject = itSingleObject->second;
				std::string filename;
				PathHelpers::GetDataBasePath(filename);
				filename.append( dbObject->GetName() );
				filename.append( ".xml");

				pugi::xml_document doc;
				if (doc.load_file( filename.c_str() ))
				{
					for (pugi::xml_node_iterator it = doc.begin(); it != doc.end(); ++it)
					{
						const pugi::xml_node& currentNode = *it;
						dbObject->GetDescriptor().Load( dbObject, &currentNode );
					}
				}
			}
		}
	}

	//No need to keep the temporary objects, they are all loaded.
	m_tempObjects.clear();
}

const CDBManager::TLibraryMap* CDBManager::GetLibrary(CStringID libraryID) const
{
	TDBMapConstIt it = m_dbMap.find(libraryID.GetUniqueID());
	if (it != m_dbMap.end())
	{
		const SDBObjectType& dbType = it->second;
		if (dbType.m_libPrefix != NULL && *(dbType.m_libPrefix) != '\0')
		{
			const TLibraryMap& libMap = it->second.m_libraryObjects;
			return &libMap;
		}
	}

	//throw an assert
	return NULL;
}

const char* CDBManager::GetLibraryPrefix(CStringID libraryID) const
{
	TDBMapConstIt it = m_dbMap.find(libraryID.GetUniqueID());
	if (it != m_dbMap.end())
	{
		const SDBObjectType& dbType = it->second;
		if (dbType.m_libPrefix != NULL && *(dbType.m_libPrefix) != '\0')
		{
			return dbType.m_libPrefix;
		}
	}

	return "";
}