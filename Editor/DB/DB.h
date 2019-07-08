#pragma once
#include "Game\Core\Callback.h"
#include "Game\Core\StringID.h"
#include "wx\notebook.h"

class wxNotebook;
class wxFrame;
class wxPanel;
class DBObjectPtrPropertyModifier;
class CTabEntityLibrary;
class CTabSingleObjects;
class CTabObjectLibraries;
class CTabEntityArchetypes;
class CTabEntityProperties;
class CTabEntityPrototypes;
class wxBookCtrlEvent;

///////////////////////////////////////////////////////////////////////////
//MainDB
///////////////////////////////////////////////////////////////////////////

class myNotebook :public wxNotebook
{
	DECLARE_EVENT_TABLE()
public:
	myNotebook(wxWindow *parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxNotebookNameStr
	);
	~myNotebook();

	///////////////////////////////////////////////////////////////////////////
	void SetTabChangedCallback(CCallback<wxBookCtrlEvent&>& callback)
	{
		m_isValid = true;
		m_onTabChangedCallback = callback;
	}

	void OnTabChange(wxBookCtrlEvent& event);

	CCallback<wxBookCtrlEvent&> m_onTabChangedCallback;
	bool m_isValid;
};

class MainDB
{
public:
    MainDB();
    ~MainDB();

    void Init( wxFrame* mainFrame );

    wxPanel* GetPanel(){ return m_panelTabs; }

    CTabEntityProperties* GetTabEntityProperties() const{ return m_tabEntityProperties; };

private:
	void OnTabChange(wxBookCtrlEvent& event);

    wxPanel* m_panelTabs;
	myNotebook* m_tabs;

    CTabSingleObjects* m_tabSingleObjects;
    CTabObjectLibraries* m_tabObjectLibraries;
	CTabEntityLibrary* m_tabEntities; 
	CTabEntityArchetypes* m_tabEntityArchetypes;
	CTabEntityProperties* m_tabEntityProperties;
	CTabEntityPrototypes* m_tabEntityPrototypes;
};