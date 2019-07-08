#pragma once

#include "Game/Core/StringID.h"
#include "Game/Core/Callback.h"
#include <wx/frame.h>

class MainScene;
class MainDB;
class wxBoxSizer;
class DBObjectPtrPropertyModifier;
class CTabEntityLibrary;
class CToolBar;

///////////////////////////////////////////////////////////////////////////////
//MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
public:
    MainFrame
	( wxWindow* parent
	, wxWindowID id = wxID_ANY
	, const wxString& title = wxEmptyString
	, const wxPoint& pos = wxDefaultPosition
	, const wxSize& size = wxSize( 1024,768 )
	, long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

    ~MainFrame();

protected:
    wxBoxSizer* m_bSizer1;
    CToolBar*   m_toolBar;
    MainScene*  m_mainScene;
    MainDB*     m_mainDB;
};