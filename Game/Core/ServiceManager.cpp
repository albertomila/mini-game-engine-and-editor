#include "Game_PCH.h"
#include "Game/Core/ServiceManager.h"


/////////////////////////////////////////////////////////////////////////////////
void CServiceManager::Init()
{
	FOR_EACH(TServiceMap, it, m_services)
	{
		it->second->Init();
	}
}

/////////////////////////////////////////////////////////////////////////////////
void CServiceManager::ShutDown()
{
	FOR_EACH(TServiceMap, it, m_services)
	{
		it->second->ShutDown();
	}
}

/////////////////////////////////////////////////////////////////////////////////
void CServiceManager::OnGameStart()
{
	FOR_EACH(TServiceMap, it, m_services)
	{
		it->second->OnGameStart();
	}
}

/////////////////////////////////////////////////////////////////////////////////
void CServiceManager::OnGameEnd()
{
	FOR_EACH(TServiceMap, it, m_services)
	{
		it->second->OnGameEnd();
	}
}

/////////////////////////////////////////////////////////////////////////////////
void CServiceManager::Update()
{
	FOR_EACH(TServiceMap, it, m_services)
	{
		it->second->Update();
	}
}