#pragma once
#include "Game\Core\Singleton.h"

class CMDGame;

///////////////////////////////////////////////////////////////////////////////
//CMainApp
///////////////////////////////////////////////////////////////////////////////
class CMainApp : public CSingleton<CMainApp>
{
public:
    CMainApp();
    ~CMainApp();

    void Init();
    bool Run();
    void Shutdown();
    
private:
    CMDGame* m_game;
};