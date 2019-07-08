#include "Editor_PCH.h"
#include "Editor/DB/TabObjectLibraries.h"

#include "Editor/DB/DBObjectPropertyGrid.h"
#include "Editor/DB/PropertyModifierHelpers.h"
#include "Editor/Main/GUITypes.h"
#include "Game/DB/DBManager.h"
#include "wx/string.h"
#include <vector>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/propgrid/property.h>
#include <wx/propgrid/propgrid.h>
#include <wx/sizer.h>
#include <wx/treectrl.h>


BEGIN_EVENT_TABLE(CTabObjectLibraries, wxDialog )
    EVT_LISTBOX(ListboxPage_Listbox, CTabObjectLibraries::OnItemSelected)
    EVT_LISTBOX(ListboxPage_Listbox, CTabObjectLibraries::OnNewLibraryObjectButtonClick)
    EVT_LISTBOX(ListboxPage_Listbox, CTabObjectLibraries::OnDeleteLibraryObjectButtonClick)
    EVT_LISTBOX(ListboxPage_Listbox, CTabObjectLibraries::OnSaveLibraryButtonClick)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////////////////////////
//LibraryItemSelectedData
///////////////////////////////////////////////////////////////////////////////
struct LibraryItemSelectedData : public wxTreeItemData
{
    const CDbObject* m_dbOBject;
};


///////////////////////////////////////////////////////////////////////////////
//CTabObjectLibraries
///////////////////////////////////////////////////////////////////////////////
CTabObjectLibraries::CTabObjectLibraries( wxNotebook* parent )
: m_parent( parent )
, m_dbPropertyGrid( NULL )
, m_listBoxLibrary( NULL )
, m_panelTabLibraryObjects( NULL )
, m_sizerTabLibraryObjects( NULL )
, m_libraryObjectsControlsSizer( NULL )
, m_libraryObjectsControlsPanel( NULL )
, m_libraryObjectsSizer( NULL )
, m_libraryObjectsPanel( NULL )
, m_treeLibraryObjects( NULL )
, m_buttonNewLibraryObject( NULL )
, m_buttonDeleteLibraryObject( NULL )
, m_buttonSaveLibraryObjects( NULL )
, m_newLibraryObjectTextField( NULL )
{

}

///////////////////////////////////////////////////////////////////////////
CTabObjectLibraries::~CTabObjectLibraries()
{
    // Disconnect Events
    m_listBoxLibrary->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CTabObjectLibraries::OnItemSelected ), NULL, this );
    m_treeLibraryObjects->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( CTabObjectLibraries::OnLibraryObjectSelected ), NULL, this );

    delete m_dbPropertyGrid;
}

///////////////////////////////////////////////////////////////////////////
void CTabObjectLibraries::Init()
{
    m_panelTabLibraryObjects = new wxPanel( m_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    m_sizerTabLibraryObjects = new wxBoxSizer( wxHORIZONTAL );

    m_listBoxLibrary = new wxListBox( m_panelTabLibraryObjects, wxID_ANY, wxPoint( -1,-1 ), wxSize( 150,250 ), 0, NULL, 0 ); 
    m_listBoxLibrary->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CTabObjectLibraries::OnItemSelected ), NULL, this );
    m_sizerTabLibraryObjects->Add( m_listBoxLibrary, 0, wxALL, 5 );

    m_libraryObjectsPanel = new wxPanel( m_panelTabLibraryObjects, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    m_libraryObjectsSizer = new wxBoxSizer( wxVERTICAL );

    //controls
    m_libraryObjectsControlsPanel = new wxPanel( m_libraryObjectsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    m_libraryObjectsControlsSizer = new wxBoxSizer( wxHORIZONTAL );

    m_newLibraryObjectTextField = new wxTextCtrl( m_libraryObjectsControlsPanel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
    m_libraryObjectsControlsSizer->Add( m_newLibraryObjectTextField, 0, wxALL, 5 );

    m_buttonNewLibraryObject = new wxButton( m_libraryObjectsControlsPanel, wxID_ANY, wxT("Add"), wxDefaultPosition, wxSize( 30,24 ) , 0 );
    m_buttonNewLibraryObject->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CTabObjectLibraries::OnNewLibraryObjectButtonClick ), NULL, this );
    m_libraryObjectsControlsSizer->Add( m_buttonNewLibraryObject, 0, wxALL, 5 );

    m_buttonDeleteLibraryObject = new wxButton( m_libraryObjectsControlsPanel, wxID_ANY, wxT("Del"), wxDefaultPosition, wxSize( 30,24 ) , 0 );
    m_buttonDeleteLibraryObject->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CTabObjectLibraries::OnDeleteLibraryObjectButtonClick ), NULL, this );
    m_libraryObjectsControlsSizer->Add( m_buttonDeleteLibraryObject, 0, wxALL, 5 );

    m_buttonSaveLibraryObjects = new wxButton( m_libraryObjectsControlsPanel, wxID_ANY, wxT("Save"), wxDefaultPosition, wxSize( 30,24 ) , 0 );
    m_buttonSaveLibraryObjects->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CTabObjectLibraries::OnSaveLibraryButtonClick ), NULL, this );
    m_libraryObjectsControlsSizer->Add( m_buttonSaveLibraryObjects, 0, wxALL, 5 );

    m_libraryObjectsControlsPanel->SetSizer( m_libraryObjectsControlsSizer );
    m_libraryObjectsControlsPanel->Layout();
    m_libraryObjectsControlsSizer->Fit( m_libraryObjectsControlsPanel );

    //tree display
    m_treeLibraryObjects = new wxTreeCtrl( m_libraryObjectsPanel, wxID_ANY, wxDefaultPosition, wxSize( 200,250 ), wxTR_DEFAULT_STYLE );
    m_treeLibraryObjects->SetWindowStyle (wxTR_HAS_BUTTONS|wxTR_NO_LINES|wxTR_HIDE_ROOT|wxTR_SINGLE);
    m_treeLibraryObjects->Connect( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( CTabObjectLibraries::OnLibraryObjectSelected ), NULL, this );
    FillLibraries();
    m_libraryObjectsSizer->Add( m_libraryObjectsControlsPanel, 0, wxALL, 5 );
    m_libraryObjectsSizer->Add( m_treeLibraryObjects, 0, wxALL, 5 );

    m_libraryObjectsPanel->SetSizer( m_libraryObjectsSizer );
    m_libraryObjectsPanel->Layout();
    m_libraryObjectsSizer->Fit( m_libraryObjectsPanel );

    m_sizerTabLibraryObjects->Add( m_libraryObjectsPanel, 0, wxALL, 5 );
    m_panelTabLibraryObjects->SetSizer( m_sizerTabLibraryObjects );
    RefreshSingleTabPanel( m_panelTabLibraryObjects, m_sizerTabLibraryObjects);

    m_parent->AddPage( m_panelTabLibraryObjects, wxT("Static libraries"), false );
}

///////////////////////////////////////////////////////////////////////////
void CTabObjectLibraries::OnItemSelected(wxCommandEvent& event)
{
    FillLibrary( event );
}

///////////////////////////////////////////////////////////////////////////
void CTabObjectLibraries::FillLibraries()
{
    U32 count( 0 );

    const CDBManager::TDBMap& dbMap = CDBManager::Get()->GetAllData();
    FOR_EACH_CONST( CDBManager::TDBMap, it, dbMap )
    {
        const CDBManager::SDBObjectType& dbType = it->second;
        CDBManager::SDBObjectType& dbObjectNonConst = const_cast< CDBManager::SDBObjectType& >( dbType );
        m_listBoxLibrary->Insert( wxString( dbType.m_libPrefix ), count, &dbObjectNonConst );
        ++count;
    }
}

///////////////////////////////////////////////////////////////////////////
void CTabObjectLibraries::CreateAndDisplayPropertyGrid( CDbObject* dbObject, wxPanel* parentPanel, wxBoxSizer* parentSizer )
{
    if( m_dbPropertyGrid )
    {
        m_dbPropertyGrid->Shutdown();
        delete m_dbPropertyGrid;
        m_dbPropertyGrid = NULL;
    }

    m_dbPropertyGrid = new DbPropertyGrid();
    m_dbPropertyGrid->Init( parentPanel, parentSizer, dbObject );

    RefreshSingleTabPanel(  parentPanel, parentSizer );
}

///////////////////////////////////////////////////////////////////////////
void CTabObjectLibraries::RefreshSingleTabPanel( wxPanel* parentPanel, wxBoxSizer* parentSizer )
{
    parentPanel->Layout();
    parentSizer->Fit( parentPanel );
}

///////////////////////////////////////////////////////////////////////////
void CTabObjectLibraries::FillLibrary( wxCommandEvent& event )
{
    CDBManager::SDBObjectType* dbObjectNonConst = static_cast<CDBManager::SDBObjectType*>( event.GetClientData() );
    if( !dbObjectNonConst )
    {
        return;
    }

    m_tempLibraryObjects.clear();
    m_treeLibraryObjects->DeleteAllItems();

    wxTreeItemId rootId = m_treeLibraryObjects->AddRoot(wxString( dbObjectNonConst->m_libPrefix ), -1, -1, NULL );

    CDBManager::TLibraryMap& libraryMap = dbObjectNonConst->m_libraryObjects;
    m_tempLibraryObjects = libraryMap;

    U32 count( 0 );
    FOR_EACH_CONST( CDBManager::TLibraryMap, itLibrary, libraryMap )
    {
        const CDbObject* dbObject = itLibrary->second;

        LibraryItemSelectedData* data = new LibraryItemSelectedData();
        data->m_dbOBject = dbObject;

        m_treeLibraryObjects->InsertItem( rootId, count, wxString( dbObject->GetName() ),-1, -1, data );
        ++count;
    }
}

///////////////////////////////////////////////////////////////////////////
void CTabObjectLibraries::OnLibraryObjectSelected( wxTreeEvent& event )
{
    wxTreeItemId itemId = event.GetItem();

    LibraryItemSelectedData* data = static_cast<LibraryItemSelectedData*>( m_treeLibraryObjects->GetItemData(itemId) );

    CDbObject* dbObjectNonConst = const_cast< CDbObject* >( data->m_dbOBject );    
    if( !dbObjectNonConst )
    {
        return;
    }

    CreateAndDisplayPropertyGrid( dbObjectNonConst, m_panelTabLibraryObjects, m_sizerTabLibraryObjects );
}

///////////////////////////////////////////////////////////////////////////
void CTabObjectLibraries::OnNewLibraryObjectButtonClick( wxCommandEvent& event )
{
    wxString& objectName = m_newLibraryObjectTextField->GetValue();
    if( objectName.empty() )
    {
        return;
    }

    CStringID newObjectID( objectName.c_str() );

    if( m_tempLibraryObjects.find( newObjectID.GetUniqueID() ) != m_tempLibraryObjects.end() )
    {
        return;
    }

    void* clientData = m_listBoxLibrary->GetClientData(m_listBoxLibrary->GetSelection() );
    if( clientData )
    {
        CDBManager::SDBObjectType* data = static_cast< CDBManager::SDBObjectType* >( clientData );
        if( data )
        {
            CDbObject* dbObject = data->m_creator();
            dbObject->SetName( objectName.c_str() );
            dbObject->SetKey( newObjectID );
            m_tempLibraryObjects[ newObjectID.GetUniqueID() ] = dbObject;

            LibraryItemSelectedData* data = new LibraryItemSelectedData();
            data->m_dbOBject = dbObject;
            U32 count2 = m_treeLibraryObjects->GetChildrenCount( m_treeLibraryObjects->GetRootItem() );
            m_treeLibraryObjects->InsertItem( m_treeLibraryObjects->GetRootItem(), count2 , objectName,-1, -1, data );
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void CTabObjectLibraries::OnDeleteLibraryObjectButtonClick( wxCommandEvent& event )
{
    LibraryItemSelectedData* data = static_cast<LibraryItemSelectedData*>( m_treeLibraryObjects->GetItemData( m_treeLibraryObjects->GetSelection() ) );
    const CDbObject* dbObject = data->m_dbOBject;

    m_tempLibraryObjects.erase( dbObject->GetKey().GetUniqueID() );

    m_treeLibraryObjects->Delete( m_treeLibraryObjects->GetSelection() );
}

///////////////////////////////////////////////////////////////////////////
void CTabObjectLibraries::OnSaveLibraryButtonClick( wxCommandEvent& event )
{
    if( m_dbPropertyGrid )
    {
        m_dbPropertyGrid->Save();

        CDBManager::SDBObjectType* data = static_cast< CDBManager::SDBObjectType* >( m_listBoxLibrary->GetClientData(m_listBoxLibrary->GetSelection() ) );
        if( data )
        {
            CDBManager::TLibraryMap& libraryMap = data->m_libraryObjects;
            libraryMap = m_tempLibraryObjects;
        }

        CDBManager::Get()->SaveAllDb();
    }
}