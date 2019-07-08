#include "Editor_PCH.h"
#include "Editor/DB/TabEntityArchetypes.h"
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include "Game/entity/Entity.h"
#include "Editor/DB/DBObjectPropertyGrid.h"
#include "wx/event.h"
#include <wx/listbox.h>
#include "Game/entity/EntityRegistry.h"
#include "Game/Gameplay/Level.h"
#include "Editor/Canvas/MainGLCanvas.h"
#include "Game/Core/StringToType.h"
#include "Game/Core/TypeToString.h"
#include "Game/entity/EntityLibrary.h"

BEGIN_EVENT_TABLE(CTabEntityArchetypes, wxDialog )
END_EVENT_TABLE()


///////////////////////////////////////////////////////////////////////////////
//CTabEntityLibrary
///////////////////////////////////////////////////////////////////////////////
CTabEntityArchetypes::CTabEntityArchetypes( wxNotebook* parent )
: m_parent( parent )
, m_mainPanel( NULL )
, m_mainSizer( NULL )
{
}

CTabEntityArchetypes::~CTabEntityArchetypes()
{
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityArchetypes::Init()
{
    m_mainPanel = new wxPanel( m_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    m_mainSizer = new wxBoxSizer( wxHORIZONTAL );


	m_entityLibrariesListBox = new wxListBox(m_mainPanel, wxID_ANY, wxPoint(-1, -1), wxSize(150, 250), 0, NULL, 0);
	m_entityLibrariesListBox->DragAcceptFiles(true);
	m_mainSizer->Add(m_entityLibrariesListBox, 0, wxALL, 5);


	m_buttonAddEntity = new wxButton(m_mainPanel, wxID_ANY, wxT("Add"), wxDefaultPosition, wxSize(30, 24), 0);
	m_buttonAddEntity->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CTabEntityArchetypes::OnAddEntityButtonClick), NULL, this);
	m_mainSizer->Add(m_buttonAddEntity, 0, wxALL, 5);

    m_mainPanel->SetSizer( m_mainSizer );
	m_parent->AddPage(m_mainPanel, wxT("Entity archetypes"), false);

	FillEntityArchetypes();
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityArchetypes::RefreshSingleTabPanel( wxPanel* parentPanel, wxBoxSizer* parentSizer )
{
    parentPanel->Layout();
	parentSizer->Fit(parentPanel);
}

///////////////////////////////////////////////////////////////////////////
void CTabEntityArchetypes::FillEntityArchetypes()
{
	U32 count(0);
	const CEntityRegistry::TEntityTypesMap& entityTypes = CEntityRegistry::Get()->GetEntityTypes();
	FOR_EACH_CONST(CEntityRegistry::TEntityTypesMap, it, entityTypes)
	{
		const CEntityRegistry::ITypeEntity* typeEntity = it->second;
		CEntityRegistry::ITypeEntity* typeEntityNonConst = const_cast<CEntityRegistry::ITypeEntity*>(typeEntity);
		m_entityLibrariesListBox->Insert(wxString(typeEntity->GetEntityType().c_str()), count, typeEntityNonConst);
		++count;
	}
}


///////////////////////////////////////////////////////////////////////////
void CTabEntityArchetypes::OnAddEntityButtonClick(wxCommandEvent& event)
{
	int selection = m_entityLibrariesListBox->GetSelection();
	if (selection >= 0)
	{
		void* clientData = m_entityLibrariesListBox->GetClientData(selection);
		CEntityRegistry::ITypeEntity* data = static_cast<CEntityRegistry::ITypeEntity*>(clientData);
		if (data)
		{
			CStringID entityType = data->GetEntityType();

			std::string entityName(entityType.c_str());
			entityName.append("_");
			U32 nextEntitySufix = CLevel::Get()->GetNextEntityCountSufix(entityType);
			entityName.append(StringUtils::TypeToString(nextEntitySufix));

			CEntity* newEntity = CEntityRegistry::Get()->CreateNewEntity(entityType, CStringID(entityName.c_str()));
			newEntity->SetName(entityName);

			CLevel::Get()->AddEntity(newEntity);
			CMainGLCanvas::RequestReloadTextures();

			return;
		}
	}
}