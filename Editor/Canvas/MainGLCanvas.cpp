#include "Editor_PCH.h"
#include "Editor/Canvas/MainGLCanvas.h"

#include "Editor/Canvas/Drawable.h"
#include "Editor/Canvas/Image.h"
#include "Game/Core/Math.h"
#include "Game/Core/Path.h"
#include "Game/Core/StringToType.h"
#include "Game/Core/TypeToString.h"
#include "Game/Gameplay/Level.h"
#include "Game/Utils/Random.h"
#include "Game/entity/Entity.h"
#include "Game/entity/EntityLibrary.h"
#include "MDGame/Test/GraphicComponent.h"
#include "wx/glcanvas.h"
#include "wx/sizer.h"
#include "wx/wx.h"
#include <vector>
#include <winbase.h>

// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <windows.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif


bool CMainGLCanvas::ms_reload = false;
bool CMainGLCanvas::ms_gridActivated = false;

BEGIN_EVENT_TABLE(CMainGLCanvas, wxGLCanvas)
    EVT_MOTION(CMainGLCanvas::mouseMoved)
    EVT_LEFT_DOWN(CMainGLCanvas::mouseDown)
    EVT_LEFT_UP(CMainGLCanvas::mouseReleased)
    EVT_RIGHT_DOWN(CMainGLCanvas::mouseRightDown)
    EVT_RIGHT_UP(CMainGLCanvas::mouseRightUp)
    EVT_LEAVE_WINDOW(CMainGLCanvas::mouseLeftWindow)
    EVT_SIZE(CMainGLCanvas::resized)
    EVT_KEY_DOWN(CMainGLCanvas::keyPressed)
    EVT_KEY_UP(CMainGLCanvas::keyReleased)
    EVT_MOUSEWHEEL(CMainGLCanvas::mouseWheelMoved)
    EVT_PAINT(CMainGLCanvas::OnPaint)
    EVT_IDLE(CMainGLCanvas::OnIdle)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////
CMainGLCanvas::CMainGLCanvas(wxFrame* parent, const wxGLAttributes& attributes)
: wxGLCanvas(parent, wxID_ANY, NULL, wxDefaultPosition, wxSize(1024,600 ), 0, wxT("GLCanvas"))
, m_projectionScale( 1.f )
, m_projectionPosX( 0.f )
, m_projectionPosY( 0.f )
, m_selectedEntity( NULL )
, m_gridFactorX( 100.f )
, m_gridFactorY( 100.f)
, m_isMouseLeftDown( false )
, m_moveEntityMultiplier( 1 )
, m_usingPan( false )
{
	count = 0;
}

void CMainGLCanvas::ProjectMouseCoords( const SMousePos& mousePos, SProjectionPos& out)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    // GLdouble posX, posY, posZ;

    winX = (float)mousePos.x;
    winY = (float)m_viewport[3] - (float)mousePos.y;
    glReadPixels( mousePos.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, m_modelview, m_projection, m_viewport, &out.x, &out.y, &out.z);
	
	/*
	std::string output;
	output.append("viewport:" + StringUtils::TypeToString(viewport[0]) + ", " + StringUtils::TypeToString(viewport[1]) + ", " + StringUtils::TypeToString(viewport[2]) + ", " + StringUtils::TypeToString(viewport[3]));
	output.append("\n");
	output.append("win:" + StringUtils::TypeToString(winX) + ", " + StringUtils::TypeToString(winY) + ", " + StringUtils::TypeToString(winZ));
	output.append("\n");
	output.append("out:" + StringUtils::TypeToString((float)out.x) + ", " + StringUtils::TypeToString((float)out.y) + ", " + StringUtils::TypeToString((float)out.z));
	output.append("\n");
	OutputDebugString(std::wstring(output.begin(), output.end()).c_str());
	*/
}
 
///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::mouseDown(wxMouseEvent& event)
{
    m_selectedEntity = NULL;
    m_isMouseLeftDown = true;

    SetFocus();


    SMousePos mousePos;
    event.GetPosition( &mousePos.x, &mousePos.y);

    SProjectionPos mouseProjectionPos;
    ProjectMouseCoords( mousePos, mouseProjectionPos );

    const std::vector<CEntity*>& entities = CLevel::Get()->GetEntities();
    FOR_EACH_CONST( std::vector<CEntity*>, it, entities )
    {
        CEntity* entity = *it;

        TSpriteMap::iterator itFind = m_spriteMap.find(  entity->GetKey().GetUniqueID()  );
        if( itFind != m_spriteMap.end() )
        {
            CDrawable* sprite = itFind->second;
            if( sprite->IsInside( mouseProjectionPos.x, mouseProjectionPos.y ) )
            {
                m_selectedEntity = entity;
                
                m_mouseEntityOffset = vector2( mouseProjectionPos.x- entity->GetPos()[VECT_X], mouseProjectionPos.y - entity->GetPos()[VECT_Y] );

                break;
            }
        }
    }

    m_onEntitySelectedShowPropertiesCallback.Call( m_selectedEntity );
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::mouseReleased(wxMouseEvent& event)
{
	m_selectedEntity = NULL;
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::mouseRightDown(wxMouseEvent& event)
{
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::mouseRightUp(wxMouseEvent& event)
{
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::mouseWheelMoved(wxMouseEvent& event)
{
    if( event.GetWheelRotation() > 0 )
    {
        m_projectionScale +=0.2f;
    }
    else
    {
        m_projectionScale -=0.2f;
    }

    Utils::Clamp( m_projectionScale, 0.05f, 10.f );

    DEBUG_END_SCOPE;
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::mouseLeftWindow(wxMouseEvent& event)
{
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::keyPressed(wxKeyEvent& event)
{
    if( m_selectedEntity )
    {
        vector3 pos = m_selectedEntity->GetPos();

        if( event.GetKeyCode() == WXK_SHIFT)
        {
            m_moveEntityMultiplier = 100;
        }

        if( event.GetKeyCode() == WXK_NUMPAD_LEFT || event.GetKeyCode() == WXK_LEFT )
        {
            pos[VECT_X] -= m_moveEntityMultiplier;
            m_selectedEntity->SetPos( pos );
        }
        if( event.GetKeyCode() == WXK_NUMPAD_RIGHT || event.GetKeyCode() == WXK_RIGHT )
        {
            pos[VECT_X] += m_moveEntityMultiplier;
            m_selectedEntity->SetPos( pos );
        }
        if( event.GetKeyCode() == WXK_NUMPAD_UP || event.GetKeyCode() == WXK_UP )
        {
            pos[VECT_Y] += m_moveEntityMultiplier;
            m_selectedEntity->SetPos( pos );
        }
        if( event.GetKeyCode() == WXK_NUMPAD_DOWN || event.GetKeyCode() == WXK_DOWN )
        {
            pos[VECT_Y] -= m_moveEntityMultiplier;
            m_selectedEntity->SetPos( pos );
        }

        if( event.GetKeyCode() == WXK_DELETE )
        {
            TSpriteMap::iterator itFind = m_spriteMap.find(  m_selectedEntity->GetKey().GetUniqueID()  );
            if( itFind != m_spriteMap.end() )
            {
                delete itFind->second;
                m_spriteMap.erase( itFind );
            }
            CLevel::Get()->RemoveEntity( m_selectedEntity );
            delete m_selectedEntity;
            m_selectedEntity = NULL;
        }
    }
    else
    {
        if( event.GetKeyCode() == WXK_NUMPAD_LEFT || event.GetKeyCode() == WXK_LEFT )
        {
            m_projectionPosX -= 100;
        }
        if( event.GetKeyCode() == WXK_NUMPAD_RIGHT || event.GetKeyCode() == WXK_RIGHT )
        {
            m_projectionPosX += 100;
        }
        if( event.GetKeyCode() == WXK_NUMPAD_UP || event.GetKeyCode() == WXK_UP )
        {
            m_projectionPosY += 100;
        }
        if( event.GetKeyCode() == WXK_NUMPAD_DOWN || event.GetKeyCode() == WXK_DOWN )
        {
            m_projectionPosY -= 100;
        }
        if( event.GetKeyCode() == WXK_SPACE )
        {
            m_usingPan = true;
        }
    }
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::keyReleased(wxKeyEvent& event)
{
    if( event.GetKeyCode() == WXK_SHIFT)
    {
        m_moveEntityMultiplier = 1;
    }
    if( event.GetKeyCode() == WXK_SPACE )
    {
        m_usingPan = false;
    }
}


// some useful events to use
void CMainGLCanvas::mouseMoved(wxMouseEvent& event)
{
    SMousePos currentMousePos;
    event.GetPosition( &(currentMousePos.x), &(currentMousePos.y) );

    SProjectionPos currentMouseProjectionPos;
    ProjectMouseCoords( currentMousePos, currentMouseProjectionPos);

    if( m_usingPan )
    {
        m_projectionPosX += ( m_mouseProjectionPos.x - currentMouseProjectionPos.x );
        m_projectionPosY += ( m_mouseProjectionPos.y - currentMouseProjectionPos.y );
    }

    if( m_isMouseLeftDown && m_selectedEntity )
    {
        if( ms_gridActivated )
        {
            double posGridX = floor(currentMouseProjectionPos.x / m_gridFactorX) * m_gridFactorX;
            double posGridY = floor(currentMouseProjectionPos.y / m_gridFactorY) * m_gridFactorY;
            m_selectedEntity->SetPos( vector3( posGridX, posGridY, 0.f) );
        }
        else
		{
			m_selectedEntity->SetPos(vector3(currentMouseProjectionPos.x - m_mouseEntityOffset[VECT_X], currentMouseProjectionPos.y - m_mouseEntityOffset[VECT_Y], 0.f));
		}
    }

    m_mousePos = currentMousePos;
    //m_mouseProjectionPos = currentMouseProjectionPos;
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::OnIdle(wxIdleEvent &event)
{
    ProjectMouseCoords( m_mousePos, m_mouseProjectionPos);
    if( ms_reload )
    {
        ms_reload = false;
        
        //delete sprites
        FOR_EACH_CONST( TSpriteMap, itSprite, m_spriteMap )
        {
            delete itSprite->second;
        }
        m_spriteMap.clear();

        //load sprites
        const std::vector<CEntity*>& entities = CLevel::Get()->GetEntities();
        FOR_EACH_CONST( std::vector<CEntity*>, it, entities )
        {
            CEntity* entity = *it;
            const CGraphicComponent* graphicComponent = entity->GetComponent<CGraphicComponent>();
            if( graphicComponent )
            {
				std::string filename = graphicComponent->GetTextureFile().m_filename;
				if (filename.empty())
				{
					PathHelpers::GetSpecificPath("data/sprites/spritebase.jpg", filename);
				}

                CDrawable* sprite = new CDrawable(  wxString(filename).c_str() );
                sprite->setHotspot( graphicComponent->GetPivot()[VECT_X], graphicComponent->GetPivot()[VECT_Y] );

                m_spriteMap[ entity->GetKey().GetUniqueID() ] = sprite;
            }
        }
    }
    static float rotation = 0.0f;

    //update pos sprites
    const std::vector<CEntity*>& entities = CLevel::Get()->GetEntities();

    FOR_EACH_CONST( std::vector<CEntity*>, it, entities )
    {
        CEntity* entity = *it;
        CDrawable* sprite = m_spriteMap[ entity->GetKey().GetUniqueID() ];

		//sprite->IsInside(m_mouseProjectionPos.x, m_mouseProjectionPos.y);
		vector3 pos = entity->GetPos();
		sprite->move(pos[VECT_X], pos[VECT_Y]);
    }

    Refresh();
    /* Force the redraw immediately, gets the gfx card to its max */
    event.RequestMore();
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::resized(wxSizeEvent& evt)
{
    //-_- wxGLCanvas::OnSize(evt);
 
    Refresh();
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glClearDepth(1.0f);	// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 
    glEnable(GL_COLOR_MATERIAL);
 
    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    float ratio_w_h = (float)(bottomrigth_x-topleft_x)/(float)(bottomrigth_y-topleft_y);
    gluPerspective(45 /*view angle*/, ratio_w_h, 0.1 /*clip close*/, 200 /*clip far*/);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::prepare2DViewport(int x, int y, int width, int height)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glEnable(GL_TEXTURE_2D);   // textures
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glViewport(x, y, width-x, height-y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glScaled(m_projectionScale, m_projectionScale, 1);
    gluOrtho2D
    (
          x     + m_projectionPosX
        , width + m_projectionPosX
        , y     + m_projectionPosY
        , height+ m_projectionPosY
    );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////
int CMainGLCanvas::getWidth()
{
    return GetSize().x;
}

///////////////////////////////////////////////////////////////////////////
int CMainGLCanvas::getHeight()
{
    return GetSize().y;
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::RenderMousePos()
{
    glLoadIdentity();
    glLineWidth(1.0); 
    glColor3f(0.0, 1.0, 0.0);

    glPushMatrix();
        glTranslatef(m_mouseProjectionPos.x,m_mouseProjectionPos.y,0);
        glBegin(GL_LINES);
            glVertex3f(-10, 0, 0.0);
            glVertex3f(10, 0, 0.0);
            glVertex3f(0, -10, 0.0);
            glVertex3f(0, 10, 0.0);
        glEnd();
    glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::RenderGrid()
{
   // if( ms_gridActivated )
    {
        float maxLoops = 50;
        float maxDistanceGrid = 2000;

        glLoadIdentity();
        glLineWidth(0.1f); 
        glColor3f(0.0, 0.5, 0.0);

        //double posGridX = floor(m_mouseProjectionPos.x / (m_gridFactorX * 2.0f)) * (m_gridFactorX * 2.0f);
        //double posGridY = floor(m_mouseProjectionPos.y / (m_gridFactorY * 2.0f)) * (m_gridFactorY * 2.0f);
        //glTranslatef(posGridX,posGridY,0);

        glColor3f(0.0, 0.2, 0.0);
        glPushMatrix();
            glTranslatef(0,0,0);
            glBegin(GL_LINES);
            for( int i = -maxLoops; i < maxLoops; ++i)
            {
                glVertex3f( -maxDistanceGrid, i*m_gridFactorY, 0.0);
                glVertex3f( maxDistanceGrid, i*m_gridFactorY, 0.0);

                glVertex3f( i*m_gridFactorX, -maxDistanceGrid, 0.0);
                glVertex3f( i*m_gridFactorX, maxDistanceGrid, 0.0);
            }
            glEnd();
        glPopMatrix();
    }
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::RenderDebugSelectedEntity()
{
    if( m_selectedEntity )
    {
        glLoadIdentity();
        glLineWidth(2.0); 

        glPushMatrix();
        glTranslatef(m_selectedEntity->GetPos()[VECT_X], m_selectedEntity->GetPos()[VECT_Y], 0);
        glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(-20, 0, 0.0);
            glVertex3f(100, 0, 0.0);

            glColor3f(0.0, 1.0, 0.0);
            glVertex3f(0, -20, 0.0);
            glVertex3f(0, 100, 0.0);
        glEnd();
        glPopMatrix();
    }
}

///////////////////////////////////////////////////////////////////////////
void CMainGLCanvas::OnPaint( wxPaintEvent& event )
{
	/*
	std::string output;
	output.append("On paint :" + StringUtils::TypeToString(count));
	OutputDebugString(std::wstring(output.begin(), output.end()).c_str());
	++count;
	*/
    if(!IsShown()) return;

	wxGLContext glContext(this);
	glContext.SetCurrent(*this);
	SetCurrent(glContext);

    //wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    // render loaded image
    prepare2DViewport(0,0,getWidth(), getHeight());

	RetrieveMatrix();
    RenderGrid();
    RenderMousePos();

    FOR_EACH_CONST( TSpriteMap, itSprite, m_spriteMap )
    {
		CDrawable* sprite = itSprite->second;
		if (sprite)
		{
			sprite->render();
		}
    }
    RenderDebugSelectedEntity();

    glFlush();
    SwapBuffers();
}

void CMainGLCanvas::RetrieveMatrix()
{
	glGetDoublev(GL_MODELVIEW_MATRIX, m_modelview); //get the modelview info
	glGetDoublev(GL_PROJECTION_MATRIX, m_projection); //get the projection matrix info
	glGetIntegerv(GL_VIEWPORT, m_viewport); //get the viewport info
}
