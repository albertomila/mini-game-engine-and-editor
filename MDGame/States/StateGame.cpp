#include "MDGame_PCH.h"
#include "MDGame/States/StateGame.h"

CStringID CStateGame::GetStateID() const
{ 
	return CStateGame::GetStaticStateID(); 
}

void CStateGame::Init()
{
}

bool CStateGame::Update()
{
	return false;
}

void CStateGame::Shutdown()
{
}