#pragma once

#include "Game\Core\Callback.h"
#include "Game\Core\StringID.h"
#include "Game\DB\DBManager.h"
#include "wx\dialog.h"
#include "wx\event.h"
#include <vector>

class wxFrame;
class wxCommandEvent;
class DBObjectPtrPropertyModifier;
class wxPanel;
class wxNotebook;
class wxTreeCtrl;
class wxTreeEvent;
class wxListBox;
class wxBoxSizer;
class DbPropertyGrid;
class CDbObject;
class wxTextCtrl;
class CTabEntityLibrary;
class CTabSingleObjects;
class CTabObjectLibraries;

struct LibraryEntryData
{
    U32 m_id;
};

class CTabObjectLibraries: public wxDialog
{
    DECLARE_EVENT_TABLE();
public:
    CTabObjectLibraries();
    CTabObjectLibraries( wxNotebook* parent );
    ~CTabObjectLibraries();
    void Init();

    void FillLibraries();
    void FillLibrary( wxCommandEvent& event  );
    void OnItemSelected(wxCommandEvent& event);
    void OnLibraryObjectSelected( wxTreeEvent& event );

    void OnNewLibraryObjectButtonClick( wxCommandEvent& event );
    void OnDeleteLibraryObjectButtonClick( wxCommandEvent& event );
    void OnSaveLibraryButtonClick( wxCommandEvent& event );

private:
    void CreateAndDisplayPropertyGrid( CDbObject* dbObject, wxPanel* parentPanel, wxBoxSizer* parentSizer  );
    void RefreshSingleTabPanel( wxPanel* parentPanel, wxBoxSizer* parentSizer );

    wxNotebook* m_parent;

    DbPropertyGrid* m_dbPropertyGrid;
    CCallback2<DBObjectPtrPropertyModifier*,CStringID>    m_onLibrarySelected;

    wxListBox*      m_listBoxLibrary;
    wxPanel*        m_panelTabLibraryObjects;
    wxBoxSizer*     m_sizerTabLibraryObjects;

    wxBoxSizer*     m_libraryObjectsControlsSizer;
    wxPanel*        m_libraryObjectsControlsPanel;

    wxBoxSizer*     m_libraryObjectsSizer;
    wxPanel*        m_libraryObjectsPanel;

    wxTreeCtrl*     m_treeLibraryObjects;
    wxButton*       m_buttonNewLibraryObject;
    wxButton*       m_buttonDeleteLibraryObject;
    wxButton*       m_buttonSaveLibraryObjects;
    wxTextCtrl*     m_newLibraryObjectTextField;
    CDBManager::TLibraryMap m_tempLibraryObjects;
};