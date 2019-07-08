#pragma once
#include "Game\GameDefs.h"
#include "Game\Core\StringID.h"

///////////////////////////////////////////////////////////////////////////////
//IState
///////////////////////////////////////////////////////////////////////////////
class GAME_DLL IState
{
public:
    IState(){}
    virtual ~IState(){}

    virtual void Init() = 0;
    virtual bool Update() = 0;
    virtual void Shutdown() = 0;
    virtual CStringID GetStateID() const = 0;
};