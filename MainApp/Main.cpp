// MainApp.cpp : Defines the entry point for the console application.
//

#include "MainApp_PCH.h"

#include "MainApp/MainApp.h"

int _tmain(int argc, _TCHAR* argv[])
{
    CMainApp::CreateInstance();
    CMainApp* mainApp = CMainApp::Get();
    mainApp->Init();
    bool exitApp( false );
    while ( !exitApp ) 
    {
        exitApp = mainApp->Run();
    }
    
    mainApp->Shutdown();

    CMainApp::DestroyInstance();

	return 0;
}

