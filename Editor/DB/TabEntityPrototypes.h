#pragma once
#include <vector>
#include <string>
#include "wx\dialog.h"
#include "Game\Core\Callback.h"
#include "Game\Core\StringID.h"
#include <unordered_map>

class wxNotebook;
class wxListBox;
class wxPanel;
class wxBoxSizer;
class wxCommandEvent;
class wxTreeCtrl;
class wxButton;
class wxTextCtrl;
class wxTreeEvent;
class CEntity;
class DbPropertyGrid;
class wxListEvent;

class CTabEntityPrototypes: public wxDialog
{
    DECLARE_EVENT_TABLE();
public:
	CTabEntityPrototypes();
	CTabEntityPrototypes( wxNotebook* parent );
    ~CTabEntityPrototypes();
    void Init();
	void Reload();

private:
	void FillEntityLibraries();
	void CreateAndDisplayPropertyGrid(CEntity* entity, wxPanel* parentPanel, wxBoxSizer* parentSizer);
	void OnEntityLibrarySelected(wxCommandEvent& event);
	void RefreshSingleTabPanel(wxPanel* parentPanel, wxBoxSizer* parentSizer);
	void OnAddNewInstanceButtonClick(wxCommandEvent& event);
	void OnSavePrototypeButtonClick(wxCommandEvent& event);

private:
	wxNotebook* m_parent;
	wxPanel*    m_mainPanel;
	wxBoxSizer* m_mainSizer;
	DbPropertyGrid* m_dbPropertyGrid;
	wxListBox*  m_entityLibrariesListBox;
	wxButton*   m_buttonAddNewInstance;
	wxButton*   m_buttonSaveCurrentPrototype;
};