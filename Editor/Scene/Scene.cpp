#include "Editor_PCH.h"
#include "Editor/Scene/Scene.h"

#include "Editor/Canvas/MainGLCanvas.h"
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/menu.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////
MainScene::MainScene()
    : m_mainGLCanvas( NULL )
{
}

///////////////////////////////////////////////////////////////////////////
MainScene::~MainScene()
{
    delete m_mainGLCanvas;
    m_mainGLCanvas = NULL;
}

///////////////////////////////////////////////////////////////////////////
void MainScene::Init( wxFrame* mainFrame )
{
	wxGLAttributes glAttributes;
    m_mainGLCanvas = new CMainGLCanvas(mainFrame, glAttributes);
}