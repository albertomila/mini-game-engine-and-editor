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

class CTabEntityProperties : public wxDialog
{
    DECLARE_EVENT_TABLE();
public:
    CTabEntityProperties();
    CTabEntityProperties( wxNotebook* parent );
    ~CTabEntityProperties();
    void Init();

    void OnCanvasEntitySelected( CEntity* entity );

private:
	void OnSaveEntityButtonClick(wxCommandEvent& event);
	void OnSaveAsPrototypeButtonClick(wxCommandEvent& event);
    void CreateAndDisplayPropertyGrid( CEntity* entity, wxPanel* parentPanel, wxBoxSizer* parentSizer );
    void RefreshSingleTabPanel( wxPanel* parentPanel, wxBoxSizer* parentSizer );

	void OnEntityLibrarySelected(wxCommandEvent& event);

private:
    wxNotebook* m_parent;
    wxPanel*    m_mainPanel;
    wxBoxSizer* m_mainSizer;
	wxButton*   m_buttonSaveEntity;
	wxButton*   m_buttonSaveAsPrototype;
	CEntity* m_entitySelected;

    DbPropertyGrid* m_dbPropertyGrid;
};