#pragma once
#include "Game\GameDefs.h"

#include <vector>
#include "Game/Core/StringID.h"
#include "Game/Utils/Random.h"
#include "Game/GameDefs.h"
#include <unordered_map>

class IState;

///////////////////////////////////////////////////////////////////////////////
//IStateManager
///////////////////////////////////////////////////////////////////////////////
class GAME_DLL IStateManager
{
    typedef IState* (*TStateCreator)();

protected:
    typedef std::pair<CStringID, TStateCreator > TStateCreatorPair;
    typedef std::vector< TStateCreatorPair > TStateListCreator;

	struct SFindStatePredicate;

public:
	IStateManager();
    virtual ~IStateManager();
    void PushStateCreator( CStringID stateID, TStateCreator stateCreator );
    void SelectState( CStringID stateID );
    void Init();
    virtual void ShutDown();
    void SelectNextState();

protected:
	IState* GetCurrentState() const;
	virtual void DoInit() = 0;

private:
    IState* m_currentGameState;
    TStateListCreator m_stateListCreator;
};