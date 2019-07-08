#include "Editor_PCH.h"
#include "Editor/Canvas/Drawable.h"

#include <iostream>

#ifdef __WXMAC__
#include "OpenGL/gl.h"
#else
#include <windows.h>
#include <GL/gl.h>
#endif
 
#include "wx/wx.h"
 

void CImageResourceManager::DestroyAllResources()
{
	DeleteMap(m_imageResources);
}

CImage* CImageResourceManager::GetImage(CStringID imageID)
{
	TImageResources::iterator itFind = m_imageResources.find(imageID.GetUniqueID());
	if (itFind != m_imageResources.end())
	{
		return itFind->second;
	}
	return NULL;
}

CImage* CImageResourceManager::GetImage(const wxString& path)
{
	return GetImage(CStringID(path));
}

CImage* CImageResourceManager::TryGetImage(wxString path)
{
	CStringID imageID(path);
	CImage* image = GetImage(imageID);
	if (image == NULL)
	{
		image = new CImage(path);
		m_imageResources[imageID.GetUniqueID()] = image;
	}

	return image;
}

/*
 * This is a simple class built on top of OpenGL that manages drawing images in a higher-level and quicker way.
 */

CDrawable::CDrawable( const wxString& path )
    : x(0)
    , y(0)
    , hotspotX(0)
    , hotspotY(0)
    , angle(0)
    , xscale(1)
    , yscale(1)
    , xflip(false)
    , yflip(false)
    , m_isInside( false )
{
    m_image = CImageResourceManager::Get()->TryGetImage( path );
    SetCenter( true );
}

CDrawable::CDrawable(CImage* image)
    : x(0)
    , y(0)
    , hotspotX(0)
    , hotspotY(0)
    , angle(0)
    , xscale(1)
    , yscale(1)
    , xflip(false)
    , yflip(false)
    , m_image( image )
    , m_isInside( false )
{
}
 
void CDrawable::setFlip(bool x, bool y)
{
    xflip=x;
    yflip=y;
}
 
void CDrawable::setHotspot(float x, float y)
{
    hotspotX=x;
    hotspotY=y;
}
 
void CDrawable::move(float x, float y)
{
    CDrawable::x=x;
    CDrawable::y=y;
}
 
void CDrawable::scale(float x, float y)
{
    CDrawable::xscale=x;
    CDrawable::yscale=y;
}
 
void CDrawable::scale(float k)
{
    CDrawable::xscale=k;
    CDrawable::yscale=k;
}
 
void CDrawable::setImage(CImage* image)
{
    CDrawable::m_image=image;
}
 
void CDrawable::rotate(int angle)
{
    CDrawable::angle=angle;
}

void CDrawable::SetCenter( bool center )
{
    if( center )
    {
        hotspotX = x - ( m_image->width / 2.0f );
        hotspotY = y + ( m_image->height / 2.0f );
    }
}

///////////////////////////////////////////////////////////////////////////
bool CDrawable::IsInside( double posx, double posy)
{
    float xmin = x - hotspotX;
    float xmax = x - hotspotX + m_image->width;
    float ymin = y - hotspotY;
    float ymax = y - hotspotY + m_image->height;

    if(     posx >= xmin 
        &&  posy >= ymin
        &&  posx <= xmax
        &&  posy <= ymax)
    {
        m_isInside = true;
        return true;
    }

    m_isInside = false;
    return false;
}
 
void CDrawable::render()
{
    static float m_x = 2.0f;
    m_x-=0.00005;
    assert(m_image!=NULL);

    glLoadIdentity();
    
    glPushMatrix();
        
        glTranslatef(x,y,0);
        glRotatef(angle, 0,0,1);  
        glTranslatef(-hotspotX,-hotspotY,0);
        glScalef(xscale, yscale, 1);

        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, m_image->getID()[0] );

        glBegin(GL_QUADS);
        glNormal3f( 0.0f, 0.0f, 1.0f);
        glTexCoord2f(0, 0); glVertex3f( 0, 0, 0.5f);
        glTexCoord2f(m_image->tex_coord_x-0.0001, 0); glVertex3f( m_image->width, 0, 0.5f);
        glTexCoord2f(m_image->tex_coord_x-0.0001, m_image->tex_coord_y-0.0001); glVertex3f( m_image->width, m_image->height , 0.5f);
        glTexCoord2f(0, m_image->tex_coord_y-0.0001); glVertex3f( 0, m_image->height , 0.5f);
        glEnd();
    glPopMatrix();
    


    glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(x,y,0);
        glTranslatef(-hotspotX,-hotspotY,0);
        glBegin(GL_LINES);
            glVertex3f( 0, 0, 0.0);
            glVertex3f(m_image->width, 0.0, 0.0);

            glVertex3f( m_image->width, 0, 0.0);
            glVertex3f( m_image->width, m_image->height, 0.0);

            glVertex3f( m_image->width, m_image->height, 0.0);
            glVertex3f( 0, m_image->height, 0.0);

            glVertex3f( 0, m_image->height, 0.0);
            glVertex3f( 0, 0, 0.0);
        glEnd();
    glPopMatrix();
        /*

        glBegin(GL_QUADS);


        glTexCoord2f(0, 0);
        glVertex2f( -hotspotX, m_image->height-hotspotY );

        glTexCoord2f(0, m_image->tex_coord_y);
        glVertex2f( -hotspotX, -hotspotY );

        glTexCoord2f(m_image->tex_coord_x, m_image->tex_coord_y);
        glVertex2f( m_image->width-hotspotX, -hotspotY );

        glTexCoord2f(m_image->tex_coord_x, 0);
        glVertex2f( m_image->width-hotspotX, m_image->height-hotspotY );

        glEnd();*/
    /*
    glPushMatrix();
        glTranslatef(x,y,0);

        glLineWidth(2.5); 
        if( m_isInside )
        {
            glColor3f(1.0, 0.0, 0.0);
        }
        else
        {
            glColor3f(0.0, 1.0, 0.0);
        }
        glBegin(GL_LINE_LOOP);
            glVertex3f(0, 0, 0.0);
            glVertex3f(m_image->width, 0, 0.0);
            glVertex3f(m_image->width, m_image->height, 0.0);
            glVertex3f(0, m_image->height, 0.0);
        glEnd();
    glPopMatrix();
    */
        /*
            glTexCoord2f(0, 0);
            glVertex2f( -hotspotX, m_image->height - hotspotY );

            glTexCoord2f(0, m_image->tex_coord_y);
            glVertex2f( -hotspotX, -hotspotY );

            glTexCoord2f(m_image->tex_coord_x, m_image->tex_coord_y);
            glVertex2f( m_image->width - hotspotX, -hotspotY );

            glTexCoord2f(m_image->tex_coord_x, 0);
            glVertex2f( m_image->width - hotspotX, m_image->height - hotspotY );
            */
     /*
        glBegin(GL_QUADS);
            glTexCoord2f(xflip? m_image->tex_coord_x : 0, yflip? 0 : m_image->tex_coord_y);
            glVertex2f( -hotspotX, -hotspotY );

            glTexCoord2f(xflip? 0 : m_image->tex_coord_x, yflip? 0 : m_image->tex_coord_y);
            glVertex2f( m_image->width-hotspotX, -hotspotY );

            glTexCoord2f(xflip? 0 : m_image->tex_coord_x, yflip? m_image->tex_coord_y : 0);
            glVertex2f( m_image->width-hotspotX, m_image->height-hotspotY );

            glTexCoord2f(xflip? m_image->tex_coord_x : 0, yflip? m_image->tex_coord_y : 0);
            glVertex2f( -hotspotX, m_image->height-hotspotY );
        glEnd();
        */
}