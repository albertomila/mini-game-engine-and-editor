// MDGame.h
#pragma once

#include "Game/Core/GameTimer.h"
#include "Game/Core/GameTimer.h"
#include "Game/Core/ServiceManager.h"
#include "Game/Core/StringID.h"
#include "Game/DB/DBManager.h"
#include "Game/Entity/EntityRegistry.h"
#include "Game/Gameplay/Level.h"
#include "Game/Gameplay/LevelDBObject.h"
#include "Game/entity/EntityLibrary.h"
#include "MDGame/Entity/EntityTypes.h"
#include "MDGame/MDGameDefs.h"
#include "MDGame/States/StateManager.h"
#include "MDGame/Test/DBObjectTest.h"
#include "MDGame/Test/GraphicComponent.h"
#include <iostream>

class CMDGame
{
public:

    void RegisterAll()
    {
        RegisterSingletons();
        RegisterServices();
        RegisterComponents();
        RegisterAllDB();
    }

    void UnregisterAll()
    {
        UnregisterServices();
    }

    void Init()
    {
        RegisterAll();
        
        LoadAllData();

        m_stateManager.Init();
        CServiceManager::Get()->Init();
    }

    void Shutdown()
    {
        m_stateManager.ShutDown();
        CServiceManager::Get()->ShutDown();

        UnregisterAll();
    }

    void RegisterServices()
    {
       // CServiceManager::Get()->RegisterService<CGameServiceTest>();
    }

    void UnregisterServices()
    {
      //  CServiceManager::Get()->UnregisterService<CGameServiceTest>();
    }

    void RegisterSingletons()
    {
        CGameTimer::CreateInstance();
        CServiceManager::CreateInstance();
        CEntityRegistry::CreateInstance();
        CEntityLibrary::CreateInstance();
        CLevel::CreateInstance();
    }

    void RegisterComponents()
    {
        REGISTER_ENTITY_COMPONENT( CGraphicComponent );
        REGISTER_ENTITY_COMPONENT( CPhysicsComponent );
        REGISTER_ENTITY_COMPONENT( CSoundComponent );

		CEntityRegistry* er = CEntityRegistry::Get();
		er->RegisterEntityType<TCommonEntity>(CStringID(STRINGIZE(TCommonEntity)));
    }

	void RegisterAllDB()
    {
        CDBManager::CreateInstance();
        CDBObjectTestNested::RegisterProperties();
		CDBManager::Get()->RegisterSingleObject<CDBObjectTest>( "test_register_object" );

        CDBManager::Get()->RegisterSingleLibrary<CDBObjectTest>( "test_register_library" );
        CDBManager::Get()->RegisterSingleLibrary< CLevelDBObject>( "levels" );
    }

    void LoadAllData()
    {
        CDBManager::Get()->LoadAllDb();
        CEntityLibrary::Get()->LoadAll();
    }

    ///////////////////////////////////////////////////////////////////////////
    bool Run()
    {
        CGameTimer::Get()->PreUpdate();
        CServiceManager::Get()->Update();

        bool exit = m_stateManager.Run();
        CGameTimer::Get()->PostUpdate();

        return exit;
    }

private:
    CStateManager m_stateManager;
};