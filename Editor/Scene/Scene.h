#pragma once

class wxFrame;
class CMainGLCanvas;

class MainScene
{
public:
    MainScene();
    ~MainScene();
    
    void Init( wxFrame* mainFrame );

    CMainGLCanvas* GetCanvas(){ return m_mainGLCanvas; }

private:
    CMainGLCanvas* m_mainGLCanvas;
};