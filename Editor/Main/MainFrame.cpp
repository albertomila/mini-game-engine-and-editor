#include "Editor_PCH.h"
#include "Editor/Main/MainFrame.h"

#include "Editor/Scene/Scene.h"
#include "Editor/DB/DB.h"
#include "Editor/Main/ToolBar.h"
#include "Editor/Canvas/MainGLCanvas.h"
#include "Editor/DB/TabEntityProperties.h"
#include <wx/panel.h>
#include <wx/sizer.h>

///////////////////////////////////////////////////////////////////////////
MainFrame::MainFrame
( wxWindow* parent
, wxWindowID id
, const wxString& title
, const wxPoint& pos
, const wxSize& size
, long style ) 
    : wxFrame( parent, id, title, pos, size, style )
    , m_bSizer1( NULL )
    , m_toolBar( NULL )
    , m_mainScene( NULL )
    , m_mainDB( NULL )
{
    SetSizeHints( wxDefaultSize, wxDefaultSize );

    m_bSizer1 = new wxBoxSizer( wxVERTICAL );
	
    //toolbar
    m_toolBar = new CToolBar();
    m_toolBar->Init( this );
	
    //top panel
    m_mainScene = new MainScene();
    m_mainScene->Init(this);
    m_bSizer1->Add(1024,600, 1, wxEXPAND | wxALL, 5 );

    //bottom panel
    m_mainDB = new MainDB();
    m_mainDB->Init( this );
    m_bSizer1->Add( m_mainDB->GetPanel(), 1, wxEXPAND | wxALL, 5 );
	CCallback<CEntity*> onCanvasEntitySelectedCallback(m_mainDB->GetTabEntityProperties(), &CTabEntityProperties::OnCanvasEntitySelected);
	CMainGLCanvas* mainGLCanvas = m_mainScene->GetCanvas();
	mainGLCanvas->SetOnEntitySelectedShowPropertiesCallback(onCanvasEntitySelectedCallback);

	
	SetSizer(m_bSizer1);
	Layout();
	Centre(wxBOTH);
}

///////////////////////////////////////////////////////////////////////////
MainFrame::~MainFrame()
{
    delete m_toolBar;
    delete m_mainDB;
    delete m_mainScene;
    delete m_bSizer1;
}

