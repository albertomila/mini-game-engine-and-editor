#pragma once

#include "wx/wx.h"
#include "wx/glcanvas.h"
#include <unordered_map>
#include "Game/Core/StringID.h"
#include "Game/Core/Callback.h"
#include "Game/Core/Math.h"

class CDrawable;
class CEntity;

class CMainGLCanvas : public wxGLCanvas
{
public:
    struct SMousePos
    {
        long x;
        long y;
    };
    struct SProjectionPos
    {
        double x;
        double y;
        double z;
    };
    CMainGLCanvas(wxFrame* parent, const wxGLAttributes& attributes);

    void resized(wxSizeEvent& evt);

    int getWidth();
    int getHeight();
    void OnIdle(wxIdleEvent &event);
    void OnPaint(wxPaintEvent& event);
    void prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
    void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);

    // events
    void mouseMoved(wxMouseEvent& event);
    void mouseDown(wxMouseEvent& event);
    void mouseWheelMoved(wxMouseEvent& event);
    void mouseReleased(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);
    void mouseRightDown(wxMouseEvent& event);
    void mouseRightUp(wxMouseEvent& event);

    static void RequestReloadTextures(){ ms_reload = true; }
    static void ToggleLockGrid(){ ms_gridActivated = !ms_gridActivated; }
    void SetOnEntitySelectedShowPropertiesCallback( CCallback<CEntity*>& callback ){ m_onEntitySelectedShowPropertiesCallback = callback; }

protected:
    DECLARE_EVENT_TABLE()

private:
    void RenderMousePos();
    void RenderGrid();
    void RenderDebugSelectedEntity();
    void ProjectMouseCoords( const SMousePos& mousePos, SProjectionPos& out);
	void RetrieveMatrix();

    CCallback<CEntity*> m_onEntitySelectedShowPropertiesCallback;

    typedef std::unordered_map< CStringID::IDType, CDrawable*> TSpriteMap;
    TSpriteMap m_spriteMap;

    static bool ms_reload;

    float m_projectionScale;
    CEntity* m_selectedEntity;
    float m_projectionPosX;
    float m_projectionPosY;
    SMousePos m_mousePos;
    SProjectionPos m_mouseProjectionPos;

    static bool  ms_gridActivated;
    double m_gridFactorX;
    double m_gridFactorY;
    bool m_isMouseLeftDown;
    U8 m_moveEntityMultiplier;
    float m_moveWorldMultiplier;
    bool m_usingPan;
    vector2 m_mouseEntityOffset;


	GLint m_viewport[4]; //var to hold the viewport info
	GLdouble m_modelview[16]; //var to hold the modelview info
	GLdouble m_projection[16]; //var to hold the projection matrix info
	int count;
};