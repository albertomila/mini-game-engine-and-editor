#include "Editor_PCH.h"
#include "Editor/DB/TabEntityProperties.h"

#include "Editor/Canvas/MainGLCanvas.h"
#include "Editor/DB/DBObjectPropertyGrid.h"
#include "Game/Core/StringToType.h"
#include "Game/Core/TypeToString.h"
#include "Game/Gameplay/Level.h"
#include "Game/entity/Entity.h"
#include "Game/entity/EntityLibrary.h"
#include "Game/entity/EntityRegistry.h"
#include "wx/event.h"
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>

BEGIN_EVENT_TABLE(CTabEntityProperties, wxDialog )
END_EVENT_TABLE()


///////////////////////////////////////////////////////////////////////////////
//CTabEntityLibrary
///////////////////////////////////////////////////////////////////////////////
CTabEntityProperties::CTabEntityProperties( wxNotebook* parent )
: m_parent( parent )
, m_mainPanel( NULL )
, m_mainSizer( NULL )
, m_buttonSaveEntity( NULL )
, m_entitySelected(NULL)
, m_dbPropertyGrid(NULL)
{
}

CTabEntityProperties::~CTabEntityProperties()
{
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityProperties::OnEntityLibrarySelected(wxCommandEvent& event)
{
}
///////////////////////////////////////////////////////////////////////////
void CTabEntityProperties::Init()
{
    m_mainPanel = new wxPanel( m_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    m_mainSizer = new wxBoxSizer( wxHORIZONTAL );

    m_mainPanel->SetSizer( m_mainSizer );
	m_parent->AddPage(m_mainPanel, wxT("Entity inspector"), false);
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityProperties::OnSaveEntityButtonClick( wxCommandEvent& event )
{
    if( m_dbPropertyGrid )
    {
        m_dbPropertyGrid->Save();
    }
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityProperties::OnSaveAsPrototypeButtonClick(wxCommandEvent& event)
{
	if (m_dbPropertyGrid)
	{
		m_dbPropertyGrid->Save();

		CEntityLibrary::Get()->RegisterPrototypeFrom(m_entitySelected);	
	}
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityProperties::OnCanvasEntitySelected( CEntity* entity )
{
    //display entity on property panel.
    CreateAndDisplayPropertyGrid( entity, m_mainPanel, m_mainSizer );
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityProperties::CreateAndDisplayPropertyGrid( CEntity* entity, wxPanel* parentPanel, wxBoxSizer* parentSizer )
{
    if( m_dbPropertyGrid )
    {
        m_dbPropertyGrid->Shutdown();
        delete m_dbPropertyGrid;
		m_dbPropertyGrid = NULL;
		
		m_mainPanel->RemoveChild(m_buttonSaveEntity);
		delete m_buttonSaveEntity;
		m_buttonSaveEntity = NULL;

		//Disconnect Events
		m_mainPanel->RemoveChild(m_buttonSaveAsPrototype);
		delete m_buttonSaveAsPrototype;
		m_buttonSaveAsPrototype = NULL;
    }

    if( entity )
    {
		m_entitySelected = entity;
        m_dbPropertyGrid = new DbPropertyGrid();
		m_dbPropertyGrid->Init(parentPanel, parentSizer, entity);

		m_buttonSaveEntity = new wxButton(m_mainPanel, wxID_ANY, wxT("Save"), wxDefaultPosition, wxSize(30, 24), 0);
		m_buttonSaveEntity->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CTabEntityProperties::OnSaveEntityButtonClick), NULL, this);
		m_mainSizer->Add(m_buttonSaveEntity, 0, wxALL, 5);

		m_buttonSaveAsPrototype = new wxButton(m_mainPanel, wxID_ANY, wxT("Save as new prototype"), wxDefaultPosition, wxSize(150, 24), 0);
		m_buttonSaveAsPrototype->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CTabEntityProperties::OnSaveAsPrototypeButtonClick), NULL, this);
		m_mainSizer->Add(m_buttonSaveAsPrototype, 0, wxALL, 5);
		m_mainPanel->Layout();
		m_mainSizer->Fit(m_mainPanel);

        RefreshSingleTabPanel(  parentPanel, parentSizer );
    }
	else
	{
		m_entitySelected = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityProperties::RefreshSingleTabPanel( wxPanel* parentPanel, wxBoxSizer* parentSizer )
{
    parentPanel->Layout();
	parentSizer->Fit(parentPanel);
}