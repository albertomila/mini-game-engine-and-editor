#pragma once
#include "wx\event.h"
#include "wx\dialog.h"

class wxNotebook;
class wxPanel;
class wxBoxSizer;
class DbPropertyGrid;
class CEntity;
class wxButton;
class wxCommandEvent;
class wxListBox;

class CTabEntityArchetypes : public wxDialog
{
    DECLARE_EVENT_TABLE();
public:
    CTabEntityArchetypes();
    CTabEntityArchetypes( wxNotebook* parent );
    ~CTabEntityArchetypes();
    void Init();

private:
	void OnAddEntityButtonClick(wxCommandEvent& event);
    void RefreshSingleTabPanel( wxPanel* parentPanel, wxBoxSizer* parentSizer );
	void FillEntityArchetypes();

private:
    wxNotebook* m_parent;
    wxPanel*    m_mainPanel;
    wxBoxSizer* m_mainSizer;
	wxButton*   m_buttonAddEntity;
	wxListBox*  m_entityLibrariesListBox;
};