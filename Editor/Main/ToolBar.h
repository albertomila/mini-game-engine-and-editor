#pragma once
#include "wx\dialog.h"

class wxFrame;
class wxToolBar;
class wxCommandEvent;
class wxComboBox;
class CEntity;
///////////////////////////////////////////////////////////////////////////////
//MainFrame
///////////////////////////////////////////////////////////////////////////////
class CToolBar: public wxDialog
{
    DECLARE_EVENT_TABLE();
public:
    CToolBar();

    ~CToolBar();

    void Init( wxFrame* mainFrame );

protected:
    void InitLevelsComboBox();
    void SaveLevel( wxCommandEvent& event );
    void ToggleLockGrid( wxCommandEvent& event );
    void OnComboBoxLevelSelected( wxCommandEvent& event );
	void DoLoadLevel(std::string& levelFileName, std::string& levelName);

    wxToolBar* m_toolBar;
    wxFrame* m_parentFrame;
    wxComboBox* m_levelListComboBox;

    CEntity* m_fakeEntity;
};