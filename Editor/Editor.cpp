// Editor.cpp : Defines the entry point for the console application.
//

#include "Editor_PCH.h"

#include <wx/app.h>
#include "Editor/Main/MainFrame.h"
#include "Editor/Main/LibraryFrame.h"
#include "Game/DB/DBManager.h"
#include "MDGame/MDGame.h"
#include "Editor/DB/PropertyModifierDBPointer.h"
#include "Editor/Main/FrameEntityTypePicker.h"
#include "Game/entity/EntityLibrary.h"
#include "Editor/DB/DBObjectPropertyGrid.h"
#include "Editor/Canvas/Drawable.h"

class DBObjectPtrPropertyModifier;
///////////////////////////////////////////////////////////////////////////////
/// EditorMainApp
///////////////////////////////////////////////////////////////////////////////
class EditorMainApp : public wxApp
{
    virtual bool OnInit()
	{
        m_game.Init();
        CImageResourceManager::CreateInstance();
        
        MainFrame* frame = new MainFrame( ( wxFrame *) NULL , wxID_ANY, "MonsterDrift Editor", wxPoint(50,50), wxSize(1024,1024), wxDEFAULT_FRAME_STYLE );
		frame->Show(true);

		CCallback2<DBObjectPtrPropertyModifier*, CStringID> libraryCallback(this, &EditorMainApp::CallbackDisplayLibrary);
		DbPropertyGrid::SetCallbackDisplayLibrary(libraryCallback);

		SetTopWindow(frame);
        return true;
    }

public:
    void CallbackDisplayLibrary( DBObjectPtrPropertyModifier* propertyModifier, CStringID libraryID )
    {
        m_libraryFrame = new LibraryFrame( ( wxFrame *) NULL , wxID_ANY, libraryID.c_str(), wxPoint(50,50), wxSize(400,600), wxDEFAULT_FRAME_STYLE );
        m_libraryFrame->SetLibraryID( libraryID );  
        CCallback<CStringID> libraryCallback( propertyModifier, &DBObjectPtrPropertyModifier::OnItemSelected );      
        m_libraryFrame->SetOnObjectSelectedCallback( libraryCallback );
        m_libraryFrame->Init();
        m_libraryFrame->Display();
    }

    void CallbackLibraryObjectSelected()
    {
        delete m_libraryFrame;
    }
private:
    LibraryFrame* m_libraryFrame;
    CMDGame m_game;
};

IMPLEMENT_APP( EditorMainApp )