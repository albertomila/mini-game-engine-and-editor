#pragma once
#include "wx\dialog.h"

class wxPanel;
class wxBoxSizer;
class wxNotebook;
class wxListBox;
class wxButton;
class DbPropertyGrid;
class wxCommandEvent;
class CDbObject;

class CTabSingleObjects: public wxDialog
{
    DECLARE_EVENT_TABLE();
public:
	CTabSingleObjects() {}
    CTabSingleObjects( wxNotebook* parent );
    ~CTabSingleObjects();

    void Init();
    void FillSingleObjects();
    void OnItemObjectSelected(wxCommandEvent& event);
    void OnSaveItem(wxCommandEvent& event);

private:
    void CreateAndDisplayPropertyGrid( CDbObject* dbObject, wxPanel* parentPanel, wxBoxSizer* parentSizer  );
    void RefreshSingleTabPanel( wxPanel* parentPanel, wxBoxSizer* parentSizer );

    wxNotebook*     m_parent;
    wxPanel*        m_panelTabObjects;
    wxBoxSizer*     m_sizerTabObjects;
    wxListBox*      m_listBox;
    wxButton*       m_buttonSaveDBObject;
    DbPropertyGrid* m_dbPropertyGrid;
};