#pragma once
#include "Game\States\IState.h"
#include "Game\Core\StringID.h"

///////////////////////////////////////////////////////////////////////////////
//CStateGame
///////////////////////////////////////////////////////////////////////////////
class CStateGame : public IState
{
public:
    CStateGame(){}
    virtual ~CStateGame(){}

    static CStringID GetStaticStateID(){ return CStringID( 0xCB314762, "StateGame"); }
    virtual CStringID GetStateID() const override;
    static IState* CreateState(){ return new CStateGame(); }

	virtual void Init() override;
    virtual bool Update() override;
    virtual void Shutdown() override;
};