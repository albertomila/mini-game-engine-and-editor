#include "MDGame_PCH.h"
#include "MDGame/States/StateIntro.h"

CStringID CStateIntro::GetStateID() const
{
	return CStateIntro::GetStaticStateID();
}

void CStateIntro::Init()
{
}

bool CStateIntro::Update()
{
	return false;
}

void CStateIntro::Shutdown()
{
}