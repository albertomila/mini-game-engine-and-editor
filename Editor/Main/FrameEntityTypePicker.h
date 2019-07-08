#pragma once

#include "Game/Core/Callback.h"
#include "Game/Core/StringID.h"
#include <wx/frame.h>
class wxCommandEvent;
class wxListBox;
class wxBoxSizer;

///////////////////////////////////////////////////////////////////////////////
//MainFrame
///////////////////////////////////////////////////////////////////////////////
class CFrameEntityTypePicker : public wxFrame 
{
    DECLARE_EVENT_TABLE();
public:
    CFrameEntityTypePicker
	( wxWindow* parent
	, wxWindowID id = wxID_ANY
	, const wxString& title = wxEmptyString
	, const wxPoint& pos = wxDefaultPosition
	, const wxSize& size = wxSize( 1024,768 )
	, long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

    ~CFrameEntityTypePicker();

    void Init();
    void Display();
    virtual void OnEntityTypeSelected( wxCommandEvent& event );
    void SetOnEntityTypeSelectedCallback( CCallback<CStringID> onEntityTypeSelectedCallback ){ m_onEntityTypeSelectedCallback = onEntityTypeSelectedCallback; }

protected:
    wxListBox* m_entityTypesListBox;
    wxBoxSizer* m_bSizer1;
    CCallback<CStringID> m_onEntityTypeSelectedCallback;
};