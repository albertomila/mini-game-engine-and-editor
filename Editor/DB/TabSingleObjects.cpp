#include "Editor_PCH.h"
#include "Editor/DB/TabSingleObjects.h"

#include "Editor/DB/DBObjectPropertyGrid.h"
#include "Editor/Main/GUITypes.h"
#include "Game/DB/DBManager.h"
#include "wx/string.h"
#include <vector>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/propgrid/propgrid.h>
#include <wx/sizer.h>

BEGIN_EVENT_TABLE(CTabSingleObjects, wxDialog )
    EVT_LISTBOX(ListboxPage_Listbox, CTabSingleObjects::OnItemObjectSelected)
    EVT_LISTBOX(ListboxPage_Listbox, CTabSingleObjects::OnSaveItem)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
//CTabEntityLibrary
///////////////////////////////////////////////////////////////////////////////

CTabSingleObjects::CTabSingleObjects( wxNotebook* parent )
    : m_parent( parent )
    , m_panelTabObjects( NULL )
    , m_sizerTabObjects( NULL )
    , m_listBox( NULL )
    , m_buttonSaveDBObject( NULL )
    , m_dbPropertyGrid( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////
CTabSingleObjects::~CTabSingleObjects()
{
    m_listBox->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CTabSingleObjects::OnItemObjectSelected ), NULL, this );
    delete m_dbPropertyGrid;
}

///////////////////////////////////////////////////////////////////////////
void CTabSingleObjects::Init()
{
    ///////////////////////////////////////////////////////////////////////////
    //tab objects
    ///////////////////////////////////////////////////////////////////////////
    m_panelTabObjects = new wxPanel( m_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    m_sizerTabObjects = new wxBoxSizer( wxHORIZONTAL );

    //objects listbox
    m_listBox = new wxListBox( m_panelTabObjects, wxID_ANY, wxPoint( -1,-1 ), wxSize( 150,250 ), 0, NULL, 0 ); 
    m_listBox->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CTabSingleObjects::OnItemObjectSelected ), NULL, this );
    FillSingleObjects();
    m_sizerTabObjects->Add( m_listBox, 0, wxALL, 5 );

    m_buttonSaveDBObject = new wxButton( m_panelTabObjects, wxID_ANY, wxT("Save"), wxDefaultPosition, wxSize( 30,24 ) , 0 );
    m_buttonSaveDBObject->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CTabSingleObjects::OnSaveItem ), NULL, this );
    m_sizerTabObjects->Add( m_buttonSaveDBObject, 0, wxALL, 5 );

    m_panelTabObjects->SetSizer( m_sizerTabObjects );
    RefreshSingleTabPanel( m_panelTabObjects, m_sizerTabObjects);

    m_parent->AddPage( m_panelTabObjects, wxT("Static objects"), true );
}

///////////////////////////////////////////////////////////////////////////
void CTabSingleObjects::FillSingleObjects()
{
    U32 count( 0 );

    const CDBManager::TDBMap& dbMap = CDBManager::Get()->GetAllData();
    FOR_EACH_CONST( CDBManager::TDBMap, it, dbMap )
    {
        const CDBManager::SDBObjectType& dbType = it->second;

        const CDBManager::TSingleObjectMap& objectMap = dbType.m_singleObjects;
        FOR_EACH_CONST( CDBManager::TSingleObjectMap, itSingleObject, objectMap )
        {
            const CDbObject* dbObject = itSingleObject->second;
            CDbObject*  dbObjectNonConst = const_cast< CDbObject* >( dbObject );
            m_listBox->Insert( wxString( dbObject->GetName() ), count, dbObjectNonConst );
            ++count;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void CTabSingleObjects::OnSaveItem(wxCommandEvent& event)
{
    if( m_dbPropertyGrid )
    {
        m_dbPropertyGrid->Save();

        CDBManager::Get()->SaveAllDb();
    }
}

///////////////////////////////////////////////////////////////////////////
void CTabSingleObjects::OnItemObjectSelected(wxCommandEvent& event)
{
    CDbObject*  dbObjectNonConst = static_cast< CDbObject* >(  event.GetClientData() );    
    if( !dbObjectNonConst )
    {
        return;
    }

    CreateAndDisplayPropertyGrid( dbObjectNonConst, m_panelTabObjects, m_sizerTabObjects );
}

///////////////////////////////////////////////////////////////////////////
void CTabSingleObjects::CreateAndDisplayPropertyGrid( CDbObject* dbObject, wxPanel* parentPanel, wxBoxSizer* parentSizer )
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
void CTabSingleObjects::RefreshSingleTabPanel( wxPanel* parentPanel, wxBoxSizer* parentSizer )
{
    parentPanel->Layout();
    parentSizer->Fit( parentPanel );
}
