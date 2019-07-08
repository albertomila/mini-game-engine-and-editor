#include "Editor_PCH.h"
#include "Editor/DB/TabEntityPrototypes.h"



#include "Editor/Canvas/MainGLCanvas.h"
#include "Editor/DB/DBObjectPropertyGrid.h"
#include "Editor/Main/GUITypes.h"
#include "Editor/libs/pugixml/pugixml.hpp"
#include "Game/Core/Algorithm.h"
#include "Game/Core/Math.h"
#include "Game/Core/StringToType.h"
#include "Game/Core/TypeToString.h"
#include "Game/Entity/EntityRegistry.h"
#include "Game/Gameplay/Level.h"
#include "Game/Utils/Random.h"
#include "Game/entity/Entity.h"
#include "Game/entity/EntityLibrary.h"
#include "wx/event.h"
#include "wx/listbase.h"
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/treectrl.h>

BEGIN_EVENT_TABLE(CTabEntityPrototypes, wxDialog )
    /*EVT_LISTBOX(ListboxPage_Listbox, CTabEntityPrototypes::OnEntityLibrarySelected)
    EVT_LISTBOX(ListboxPage_Listbox, CTabEntityPrototypes::OnNewLibraryEntityButtonClick)
    EVT_LISTBOX(ListboxPage_Listbox, CTabEntityPrototypes::OnDeleteLibraryEntityButtonClick)
    EVT_LISTBOX(ListboxPage_Listbox, CTabEntityPrototypes::OnSaveEntityLibraryButtonClick)*/
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
//SLibraryEntitySelectedData
///////////////////////////////////////////////////////////////////////////////
struct SPrototypeSelectedData
{
	CStringID m_prototypeID;
};

///////////////////////////////////////////////////////////////////////////////
//CTabEntityPrototypes
///////////////////////////////////////////////////////////////////////////////
CTabEntityPrototypes::CTabEntityPrototypes( wxNotebook* parent )
   : m_parent( parent )
   , m_dbPropertyGrid( NULL )
{
}

CTabEntityPrototypes::~CTabEntityPrototypes()
{
    delete m_dbPropertyGrid;
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityPrototypes::Init()
{
    m_mainPanel = new wxPanel( m_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    m_mainSizer = new wxBoxSizer( wxHORIZONTAL );

    m_entityLibrariesListBox = new wxListBox( m_mainPanel, wxID_ANY, wxPoint( -1,-1 ), wxSize( 150,250 ), 0, NULL, 0 ); 
    m_entityLibrariesListBox->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CTabEntityPrototypes::OnEntityLibrarySelected ), NULL, this );
    m_mainSizer->Add( m_entityLibrariesListBox, 0, wxALL, 5 );
	

	m_mainPanel->SetSizer(m_mainSizer);
	m_parent->AddPage(m_mainPanel, wxT("Entity prototypes"), false);

	FillEntityLibraries();
}

void CTabEntityPrototypes::Reload()
{
	m_entityLibrariesListBox->Clear();
	m_entityLibrariesListBox->Refresh(true);
	FillEntityLibraries();
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityPrototypes::FillEntityLibraries()
{
	U32 count(0);
	std::vector<std::string> listFiles;
	const CEntityLibrary::TEntityMap& entities = CEntityLibrary::Get()->GetEntities();
	
	FOR_EACH_CONST(CEntityLibrary::TEntityMap, it, entities)
	{
		const CEntity* entity = it->second;

		SPrototypeSelectedData* data = new SPrototypeSelectedData();
		data->m_prototypeID = entity->GetKey();
		m_entityLibrariesListBox->Insert(wxString(entity->GetName()), count, data);
	}
	
	m_entityLibrariesListBox->Refresh(true);
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityPrototypes::OnEntityLibrarySelected(wxCommandEvent& event)
{
	SPrototypeSelectedData* data = static_cast<SPrototypeSelectedData*>(event.GetClientData());
	if (data)
	{
		CEntity* entity = CEntityLibrary::Get()->GetEntity(data->m_prototypeID);

		//display entity on property panel.
		CreateAndDisplayPropertyGrid(entity, m_mainPanel, m_mainSizer);
	}
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityPrototypes::CreateAndDisplayPropertyGrid(CEntity* entity, wxPanel* parentPanel, wxBoxSizer* parentSizer)
{
	if (m_dbPropertyGrid)
	{
		m_dbPropertyGrid->Shutdown();
		delete m_dbPropertyGrid;
		m_dbPropertyGrid = NULL;

		m_mainPanel->RemoveChild(m_buttonAddNewInstance);
		delete m_buttonAddNewInstance;
		m_buttonAddNewInstance = NULL;

		m_mainPanel->RemoveChild(m_buttonSaveCurrentPrototype);
		delete m_buttonSaveCurrentPrototype;
		m_buttonSaveCurrentPrototype = NULL;
	}

	m_buttonAddNewInstance = new wxButton(m_mainPanel, wxID_ANY, wxT("Add new Instance"), wxDefaultPosition, wxSize(150, 24), 0);
	m_buttonAddNewInstance->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CTabEntityPrototypes::OnAddNewInstanceButtonClick), NULL, this);
	m_mainSizer->Add(m_buttonAddNewInstance, 0, wxALL, 5);

	m_dbPropertyGrid = new DbPropertyGrid();
	m_dbPropertyGrid->Init(parentPanel, parentSizer, entity);

	m_buttonSaveCurrentPrototype = new wxButton(m_mainPanel, wxID_ANY, wxT("Save"), wxDefaultPosition, wxSize(150, 24), 0);
	m_buttonSaveCurrentPrototype->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CTabEntityPrototypes::OnSavePrototypeButtonClick), NULL, this);
	m_mainSizer->Add(m_buttonSaveCurrentPrototype, 0, wxALL, 5);

	m_mainPanel->Layout();
	m_mainSizer->Fit(m_mainPanel);
	RefreshSingleTabPanel(parentPanel, parentSizer);
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityPrototypes::RefreshSingleTabPanel(wxPanel* parentPanel, wxBoxSizer* parentSizer)
{
	parentPanel->Layout();
	parentSizer->Fit(parentPanel);
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityPrototypes::OnSavePrototypeButtonClick(wxCommandEvent& event)
{
	m_dbPropertyGrid->Save();
	CEntityLibrary::Get()->SaveAll();
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityPrototypes::OnAddNewInstanceButtonClick(wxCommandEvent& event)
{
	int selectedItemIndex = m_entityLibrariesListBox->GetSelection();

	void* listData = m_entityLibrariesListBox->GetClientData(selectedItemIndex);
	if (listData)
	{
		SPrototypeSelectedData* data = static_cast<SPrototypeSelectedData*>(listData);
		if (data)
		{
			CEntity* entityPrototype = CEntityLibrary::Get()->GetEntity(data->m_prototypeID);
			CEntity* newEntity = CEntityLibrary::Get()->CreateNewEntityFrom(entityPrototype);

			CLevel::Get()->AddEntity(newEntity);
			CMainGLCanvas::RequestReloadTextures();
		}
	}
}