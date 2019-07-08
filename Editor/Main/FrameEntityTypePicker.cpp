#include "Editor_PCH.h"
#include "Editor\Main\FrameEntityTypePicker.h"

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
#include "Game/Entity/EntityRegistry.h"
#include "Editor/Main/GUITypes.h"

BEGIN_EVENT_TABLE(CFrameEntityTypePicker, wxFrame )
    EVT_LISTBOX(ListboxPage_Listbox, CFrameEntityTypePicker::OnEntityTypeSelected)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////
CFrameEntityTypePicker::CFrameEntityTypePicker
( wxWindow* parent
, wxWindowID id
, const wxString& title
, const wxPoint& pos
, const wxSize& size
, long style ) 
    : wxFrame( parent, id, title, pos, size, style )
    , m_bSizer1( NULL )
{

    SetSizeHints( size, size );

    m_bSizer1 = new wxBoxSizer( wxVERTICAL );

    m_entityTypesListBox = new wxListBox( this, wxID_ANY, wxPoint( -1,-1 ), wxSize( 150,250 ), 0, NULL, 0 ); 
    m_entityTypesListBox->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CFrameEntityTypePicker::OnEntityTypeSelected ), NULL, this );
    m_bSizer1->Add( m_entityTypesListBox, 0, wxALL, 5 );


    SetSizer( m_bSizer1 );
    Layout();
    Centre( wxBOTH );
}

///////////////////////////////////////////////////////////////////////////
CFrameEntityTypePicker::~CFrameEntityTypePicker()
{
}

///////////////////////////////////////////////////////////////////////////
void CFrameEntityTypePicker::OnEntityTypeSelected(wxCommandEvent& event)
{
	CEntityRegistry::ITypeEntity*  entityType = static_cast<CEntityRegistry::ITypeEntity*>(event.GetClientData());
	if (entityType)
	{
		m_onEntityTypeSelectedCallback.Call(entityType->GetEntityType());
	}

	this->Close(true);
}

///////////////////////////////////////////////////////////////////////////
void CFrameEntityTypePicker::Init()
{
    //Fill entity types
    U32 count( 0 );
    
    const CEntityRegistry::TEntityTypesMap& entityTypes = CEntityRegistry::Get()->GetEntityTypes();
    FOR_EACH_CONST( CEntityRegistry::TEntityTypesMap, it, entityTypes )
    {
        const CEntityRegistry::ITypeEntity* typeEntity = it->second;
        CEntityRegistry::ITypeEntity* typeEntityNonConst = const_cast<CEntityRegistry::ITypeEntity*>( typeEntity );
        m_entityTypesListBox->Insert( wxString( typeEntity->GetEntityType().c_str() ), count, typeEntityNonConst );
        ++count;
    }
}

///////////////////////////////////////////////////////////////////////////
void CFrameEntityTypePicker::Display()
{
    Show(true);
}

