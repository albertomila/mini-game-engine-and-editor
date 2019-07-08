#include "Editor_PCH.h"
#include "Editor/Main/LibraryFrame.h"

#include "Editor/DB/DB.h"
#include "Editor/Scene/Scene.h"
#include "Game/DB/DBManager.h"
#include "wx/treebase.h"
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/treectrl.h>

///////////////////////////////////////////////////////////////////////////
void LibraryFrame::OnLibraryObjectSelected( wxTreeEvent& event )
{
    wxTreeItemId itemId = event.GetItem();

    LibraryItemSelectedData* data = static_cast<LibraryItemSelectedData*>( m_treeCtrl->GetItemData(itemId) );
    m_onObjectSelectedCallback.Call( data->m_dbOBject->GetKey() );

    this->Close( true );
}

///////////////////////////////////////////////////////////////////////////
LibraryFrame::LibraryFrame
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
    m_treeCtrl = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 400, 600 ), wxTR_DEFAULT_STYLE );
    m_bSizer1->Add( m_treeCtrl, 0, wxALL, 5 );
    m_treeCtrl->Connect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( LibraryFrame::OnLibraryObjectSelected ), NULL, this );
    SetSizer( m_bSizer1 );
    Layout();
    Centre( wxBOTH );
}

///////////////////////////////////////////////////////////////////////////
void LibraryFrame::Init()
{
    wxString libraryName = CDBManager::Get()->GetLibraryPrefix( m_libraryID );
    wxTreeItemId rootId = m_treeCtrl->AddRoot( libraryName, -1, -1, NULL );

    const CDBManager::TLibraryMap* library = CDBManager::Get()->GetLibrary( m_libraryID );
    if( library )
    {
        U32 count( 0 );
        FOR_EACH_CONST( CDBManager::TLibraryMap, itLibrary, (*library) )
        {
            const CDbObject* dbObject = itLibrary->second;

            LibraryItemSelectedData* data = new LibraryItemSelectedData();
            data->m_dbOBject = dbObject;
            m_treeCtrl->InsertItem( rootId, count, wxString( dbObject->GetName() ),-1, -1, data );

            ++count;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void LibraryFrame::Display()
{
    Show(true);
}

///////////////////////////////////////////////////////////////////////////
LibraryFrame::~LibraryFrame()
{
    m_treeCtrl->Disconnect( wxEVT_COMMAND_TREE_ITEM_ACTIVATED, wxTreeEventHandler( LibraryFrame::OnLibraryObjectSelected ), NULL, this );
}