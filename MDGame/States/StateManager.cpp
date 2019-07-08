#include "MDGame_PCH.h"
#include "MDGame/States/StateManager.h"

CStateManager::CStateManager()
{
}

CStateManager::~CStateManager()
{
}

void CStateManager::DoInit()
{
	PushStateCreator(CStateIntro::GetStaticStateID(), &CStateIntro::CreateState);
	PushStateCreator(CStateGame::GetStaticStateID(), &CStateGame::CreateState);
	SelectState(CStateIntro::GetStaticStateID());
}

bool CStateManager::Run()
{
	return GetCurrentState()->Update();
}