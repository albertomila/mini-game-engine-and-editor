#include "Game_PCH.h"
#include "Game/States/IStateManager.h"

#include "Game/States/IState.h"
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
struct IStateManager::SFindStatePredicate
{
	SFindStatePredicate(CStringID stateIDFind)
		: m_stateIDFind(stateIDFind)
	{
	}

	bool operator () (IStateManager::TStateCreatorPair& state)
	{
		return m_stateIDFind == state.first;
	}

	CStringID m_stateIDFind;
};

///////////////////////////////////////////////////////////////////////////////
IStateManager::IStateManager()
	: m_currentGameState(NULL)
{
}

IStateManager::~IStateManager()
{
}

IState* IStateManager::GetCurrentState() const
{
	return m_currentGameState;
}

void IStateManager::PushStateCreator(CStringID stateID, TStateCreator stateCreator)
{
	m_stateListCreator.push_back(std::make_pair(stateID, stateCreator));
}
void IStateManager::SelectState(CStringID stateID)
{
	if (m_currentGameState)
	{
		m_currentGameState->Shutdown();
		delete m_currentGameState;
		m_currentGameState = NULL;
	}

	TStateListCreator::iterator itFind = std::find_if(m_stateListCreator.begin()
		, m_stateListCreator.end()
		, SFindStatePredicate(stateID));
	if (itFind != m_stateListCreator.end())
	{
		m_currentGameState = itFind->second();
	}
}

void IStateManager::Init()
{
	DoInit();
}

void IStateManager::ShutDown()
{
}

void IStateManager::SelectNextState()
{
	if (m_currentGameState)
	{
		TStateListCreator::iterator itFind = std::find_if(m_stateListCreator.begin()
			, m_stateListCreator.end()
			, SFindStatePredicate(m_currentGameState->GetStateID()));
		if (itFind != m_stateListCreator.end())
		{
			TStateListCreator::iterator itNext = itFind;
			++itNext;

			if (itNext == m_stateListCreator.end())
			{
				itNext = m_stateListCreator.begin();
			}

			SelectState(itNext->first);
		}
	}
}