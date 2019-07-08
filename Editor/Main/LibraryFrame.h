#pragma once

#include "Game/Core/StringID.h"
#include "Game\Core\Callback.h"
#include "wx\event.h"
#include "wx\treebase.h"
#include <wx/frame.h>

class MainScene;
class MainDB;
class wxBoxSizer;
class wxTreeCtrl;
class wxTreeEvent;
class CDbObject;

///////////////////////////////////////////////////////////////////////////////
//MainFrame
///////////////////////////////////////////////////////////////////////////////
struct LibraryItemSelectedData : public wxTreeItemData
{
    const CDbObject* m_dbOBject;
};

///////////////////////////////////////////////////////////////////////////////
//MainFrame
///////////////////////////////////////////////////////////////////////////////
class LibraryFrame : public wxFrame 
{
public:
    LibraryFrame
	( wxWindow* parent
	, wxWindowID id = wxID_ANY
	, const wxString& title = wxEmptyString
	, const wxPoint& pos = wxDefaultPosition
	, const wxSize& size = wxSize( 1024,768 )
	, long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

    ~LibraryFrame();

    void SetLibraryID( CStringID libraryID ){ m_libraryID = libraryID; }
    void Init();
    void Display();
    virtual void OnLibraryObjectSelected( wxTreeEvent& event );
	void SetOnObjectSelectedCallback(CCallback<CStringID>& onObjectSelectedCallback) { m_onObjectSelectedCallback = onObjectSelectedCallback; }

protected:
    wxTreeCtrl* m_treeCtrl;
    wxBoxSizer* m_bSizer1;
    CStringID   m_libraryID;
	CCallback<CStringID> m_onObjectSelectedCallback;
};