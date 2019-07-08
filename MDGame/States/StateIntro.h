#pragma once
#include "Game\States\IState.h"
#include "Game\Core\StringID.h"

///////////////////////////////////////////////////////////////////////////////
//StateIntro
///////////////////////////////////////////////////////////////////////////////
class CStateIntro : public IState
{
public:
    CStateIntro(){}
    virtual ~CStateIntro(){}

    static CStringID GetStaticStateID(){ return CStringID( 0x5D5F1029, "StateIntro"); }
	virtual CStringID GetStateID() const override;
    static IState* CreateState(){ return new CStateIntro(); }

    virtual void Init() override;
    virtual bool Update() override;
    virtual void Shutdown() override;
};