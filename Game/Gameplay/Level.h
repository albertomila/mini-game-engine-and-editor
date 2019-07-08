#pragma once
#include "Game\Core\Singleton.h"
#include "Game\Core\StringID.h"
#include "Game\Core\Callback.h"
#include "Game\Gameplay\ResourceContainer.h"
#include "Editor\libs\pugixml\pugixml.hpp"
#include <vector>
#include <string>
#include <unordered_map>

class CEntity;

class GAME_DLL CLevel : public CSingleton<CLevel>
{
public:
    typedef CCallback< CResourceContainer& > TWorldExporterCallback;
    typedef std::vector<TWorldExporterCallback> TWorldExporterVector;

	CLevel();

	virtual ~CLevel();

    void Load( std::string& levelFileName, std::string& levelName );
    void Save();

    void AddEntity( CEntity* entity );
    void RemoveEntity( CEntity* entity );
    void CleanEntities();
    const std::vector<CEntity*>& GetEntities() const{ return m_entities; }
    U32 GetNextEntityCountSufix( CStringID entityID );
    void IncreaseNextEntityCountSufix( CStringID::IDType entityID );

    void RegisterWorldExporter( const TWorldExporterCallback& callback );
    void UnregisterWorldExporter( const TWorldExporterCallback& callback );

public:
    void LoadResources( pugi::xml_node& xmlRootNode );
    void ExportResources( pugi::xml_node& xmlRootNode );

    typedef std::unordered_map< CStringID::IDType, U32> TCountEntitiesMap;
    TCountEntitiesMap m_countEntities;

    std::vector<CEntity*> m_entities;
    std::string m_levelName;
    std::string m_levelFileName;
    TWorldExporterVector m_worldExporterCallbacks;
    CResourceContainer m_resourceContainer;

};