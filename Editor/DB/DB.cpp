#include "Editor_PCH.h"
#include "Editor/DB/DB.h"

#include "Editor/DB/DBObjectPropertyGrid.h"
#include "Editor/DB/PropertyModifierHelpers.h"
#include "Editor/DB/TabEntityArchetypes.h"
#include "Editor/DB/TabEntityProperties.h"
#include "Editor/DB/TabEntityPrototypes.h"
#include "Editor/DB/TabObjectLibraries.h"
#include "Editor/DB/TabSingleObjects.h"
#include "Editor/Main/GUITypes.h"
#include "Game/DB/DBManager.h"
#include "wx/bookctrl.h"
#include "wx/string.h"
#include <vector>
#include <winbase.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/propgrid/propgrid.h>
#include <wx/sizer.h>
#include <wx/treectrl.h>

///////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(myNotebook, wxNotebook)
EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, myNotebook::OnTabChange)
END_EVENT_TABLE()


myNotebook::myNotebook(wxWindow *parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name
)
	:wxNotebook(parent, id, pos, size, style, name)
	, m_isValid(false)
{
}

myNotebook::~myNotebook()
{

}

void myNotebook::OnTabChange(wxBookCtrlEvent& event)
{
	if (m_isValid)
	{
		m_onTabChangedCallback.Call(event);
	}
}

///////////////////////////////////////////////////////////////////////////
MainDB::MainDB()
: m_panelTabs( NULL )
, m_tabs( NULL )
, m_tabEntities( NULL )
, m_tabSingleObjects( NULL )
, m_tabObjectLibraries( NULL )
, m_tabEntityProperties( NULL )
, m_tabEntityArchetypes(NULL)
{
}

///////////////////////////////////////////////////////////////////////////
MainDB::~MainDB()
{
    delete m_tabSingleObjects;
    delete m_tabObjectLibraries;
	delete m_tabEntityArchetypes;
	delete m_tabEntityPrototypes;
    delete m_tabEntityProperties;
}

///////////////////////////////////////////////////////////////////////////
void MainDB::Init( wxFrame* mainFrame )
{
    ///////////////////////////////////////////////////////////////////////////
    //main panel
    ///////////////////////////////////////////////////////////////////////////
    m_panelTabs = new wxPanel( mainFrame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* bSizerTabs = new wxBoxSizer( wxVERTICAL );

    //main tabs
    m_tabs = new myNotebook( m_panelTabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

    ///////////////////////////////////////////////////////////////////////////
    //tab objects
    ///////////////////////////////////////////////////////////////////////////

    //tab single objects
    m_tabSingleObjects = new CTabSingleObjects( m_tabs );
    m_tabSingleObjects->Init();

    //tab object libraries
    m_tabObjectLibraries = new CTabObjectLibraries( m_tabs );
    m_tabObjectLibraries->Init();

	//tab entity archetypes
	m_tabEntityArchetypes = new CTabEntityArchetypes(m_tabs);
	m_tabEntityArchetypes->Init();

    //tab entity library
    m_tabEntityProperties = new CTabEntityProperties( m_tabs );
	m_tabEntityProperties->Init();

	//tab entity prototypes
	m_tabEntityPrototypes = new CTabEntityPrototypes( m_tabs );
	m_tabEntityPrototypes->Init();

    ///////////////////////////////////////////////////////////////////////////
    //Bind main sizer and main panel
    ///////////////////////////////////////////////////////////////////////////
    bSizerTabs->Add( m_tabs, 1, wxEXPAND | wxALL, 5 );

	//m_tabs->Connect(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxBookCtrlEventHandler(MainDB::OnChangedNotebookPage), NULL, this);

    m_panelTabs->SetSizer( bSizerTabs );
    m_panelTabs->Layout();
    bSizerTabs->Fit( m_panelTabs );

	CCallback<wxBookCtrlEvent&> tabChangedCallback(this, &MainDB::OnTabChange);
	m_tabs->SetTabChangedCallback(tabChangedCallback);
}

void MainDB::OnTabChange(wxBookCtrlEvent& event)
{
	int indexSelection = event.GetSelection();
	if (indexSelection == 3)
	{
		m_tabEntityPrototypes->Reload();
	}
}