#include "Editor_PCH.h"
#include "Editor/Main/ToolBar.h"

#include "Editor/Canvas/MainGLCanvas.h"
#include "Editor/Main/GUITypes.h"
#include "Editor/libs/pugixml/pugixml.hpp"
#include "Game/Core/Path.h"
#include "Game/Gameplay/Level.h"
#include "Game/Utils/FileHelpers.h"
#include "Game/entity/Entity.h"
#include "Game/entity/EntityLibrary.h"
#include "MDGame/Entity/EntityTypes.h"
#include "MDGame/Test/GraphicComponent.h"
#include <vector>
#include <wx/event.h>
#include <wx/combobox.h>
#include <wx/frame.h>
#include <wx/toolbar.h>

struct SLevelComboBoxData : public wxClientData
{
    std::string m_fullPath;
    std::string m_fileName;
    std::string m_name;
};


BEGIN_EVENT_TABLE(CToolBar, wxDialog )
    EVT_LISTBOX(ListboxPage_Listbox, CToolBar::SaveLevel)
    EVT_LISTBOX(ListboxPage_Listbox, CToolBar::OnComboBoxLevelSelected)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////
CToolBar::CToolBar()
    : m_toolBar( NULL )
    , m_parentFrame( NULL )
    , m_levelListComboBox( NULL )
    , m_fakeEntity( NULL )
{
}

///////////////////////////////////////////////////////////////////////////
CToolBar::~CToolBar()
{
    m_parentFrame->Disconnect( wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( CToolBar::SaveLevel ) );
    m_levelListComboBox->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( CToolBar::OnComboBoxLevelSelected ), NULL, this );
    m_levelListComboBox->Clear();
    delete m_levelListComboBox;
}

///////////////////////////////////////////////////////////////////////////
void CToolBar::Init( wxFrame* mainFrame )
{
    m_parentFrame = mainFrame;

    m_toolBar = m_parentFrame->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 

    m_levelListComboBox = new wxComboBox( m_toolBar, wxID_ANY, wxT("LevelList"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
    m_toolBar->AddControl( m_levelListComboBox );    
    m_levelListComboBox->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( CToolBar::OnComboBoxLevelSelected ), NULL, this );

	std::string path;
	PathHelpers::GetSpecificPath("/data/editor/save_22x22.bmp", path);
    m_toolBar->AddTool( 1, wxT("save"), wxBitmap(path, wxBITMAP_TYPE_ANY ));

	m_parentFrame->Connect( 1, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( CToolBar::SaveLevel ) );

	std::string path2;
	PathHelpers::GetSpecificPath("/data/editor/grid-snap.bmp", path2);
    m_toolBar->AddTool( 2, wxT("lockGrid"), wxBitmap(path2, wxBITMAP_TYPE_ANY));
	m_parentFrame->Connect(2, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(CToolBar::ToggleLockGrid));


    m_toolBar->Realize(); 

    InitLevelsComboBox();
}

///////////////////////////////////////////////////////////////////////////
void CToolBar::InitLevelsComboBox()
{
    std::vector<std::string> listFiles;

	std::string levelsPath;
	PathHelpers::GetLevelsPath(levelsPath);
    FileHelpers::GetFilesInDirectory(levelsPath.c_str(), "xml", listFiles);

    FOR_EACH_CONST( std::vector<std::string>, it, listFiles )
	{
		std::string fullPath;
		PathHelpers::GetLevelsPath(fullPath);
        fullPath.append( *it );

        pugi::xml_document doc;
        if ( doc.load_file( fullPath.c_str() ))
        {
            pugi::xml_node xmlRootNode = doc.child( "level" );
            if (xmlRootNode)
            {
                pugi::xml_attribute xmlAttributeName = xmlRootNode.attribute( "name" );

                std::string name( xmlAttributeName.value() );

                SLevelComboBoxData* data = new SLevelComboBoxData();
                data->m_fullPath = fullPath;
                data->m_fileName = *it;
                data->m_name = name;

                wxString fileNameWX( name.c_str() );
                m_levelListComboBox->Append( fileNameWX, data );
            }
        }
    }
    void* clientData = m_levelListComboBox->GetClientObject( 0 );
    SLevelComboBoxData* data = static_cast< SLevelComboBoxData* >( clientData );

	m_levelListComboBox->Select(0);
	CLevel::Get()->Load(data->m_fileName, data->m_name);
	CMainGLCanvas::RequestReloadTextures();
}

///////////////////////////////////////////////////////////////////////////
void CToolBar::SaveLevel( wxCommandEvent& event )
{
    CLevel::Get()->Save();
}

///////////////////////////////////////////////////////////////////////////
void CToolBar::ToggleLockGrid( wxCommandEvent& event )
{
    CMainGLCanvas::ToggleLockGrid();
}

///////////////////////////////////////////////////////////////////////////
void CToolBar::OnComboBoxLevelSelected( wxCommandEvent& event )
{
    SLevelComboBoxData* data = static_cast< SLevelComboBoxData* >( m_levelListComboBox->GetClientObject( m_levelListComboBox->GetSelection() ) );
    if( data )
    {
		DoLoadLevel(data->m_fileName, data->m_name);
    }
}

void CToolBar::DoLoadLevel(std::string& levelFileName, std::string& levelName)
{
	CLevel::Get()->Load(levelFileName, levelName);
	CMainGLCanvas::RequestReloadTextures();
}