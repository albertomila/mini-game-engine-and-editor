#pragma once
#include "Game\States\IStateManager.h"
#include "MDGame/States/StateIntro.h"
#include "MDGame/States/StateGame.h"
#include "MDGame\MDGameDefs.h"

class MDGAME_DLL CStateManager : public IStateManager
{
public:
	CStateManager();
	virtual ~CStateManager();
	bool Run();

private:
	virtual void DoInit() override;
};