#pragma once

#include "Game\Core\Callback.h"
#include "Game\Core\StringID.h"
#include "wx\event.h"
#include "wx\dialog.h"
#include <vector>

class CDbObject;
class wxPropertyGridEvent;
class wxCommandEvent;
class wxPropertyCategory;
class CMember;
class IPropertyModifier;
class DBObjectPtrPropertyModifier;
class wxPropertyGrid;
class wxPanel;
class CEntity;

///////////////////////////////////////////////////////////////////////////
//DbPropertyGrid
///////////////////////////////////////////////////////////////////////////
class DbPropertyGrid : public wxDialog
{
    DECLARE_EVENT_TABLE();
public:
    DbPropertyGrid();
    ~DbPropertyGrid();
    
    void Init( wxPanel* parentPanel, wxBoxSizer* parentSizer, CDbObject* dbObject );
    void Init( wxPanel* parentPanel, wxBoxSizer* parentSizer, CEntity* entity );
    void Shutdown();
    void OnPropertySelected(wxPropertyGridEvent& event);
    void OnPropertyRightButton(wxPropertyGridEvent& event);
    void OnPropertySaveButtonClick( wxCommandEvent& event );
    void ComposePropertyPanel( CDbObject* dbObjectNonConst, wxPropertyCategory* parentCategory = NULL );
    void ComposePropertyPanel( CDbObject* dbObjectNonConst, const std::vector< CMember* >& memberList, wxPropertyCategory* parentCategory = NULL);
    void ComposePropertyPanel( CEntity* entity);
    void Save();

    static void SetCallbackDisplayLibrary( CCallback2<DBObjectPtrPropertyModifier*,CStringID>& callback ){ ms_onLibrarySelected = callback; }

private:
    void ParsePropertyChildren( IPropertyModifier* parentProperty );

    void Init_Begin( wxPanel* parentPanel, wxBoxSizer* parentSizer );
    void Init_End( wxPanel* parentPanel, wxBoxSizer* parentSizer );

    wxPanel*        m_parentPanel;
    wxBoxSizer*     m_parentSizer;
    wxBoxSizer*     m_currentSizer;
    wxPanel*        m_currentPanel;

    wxPropertyGrid* m_objectsPropertyGrid;
    static CCallback2<DBObjectPtrPropertyModifier*,CStringID>    ms_onLibrarySelected;
};