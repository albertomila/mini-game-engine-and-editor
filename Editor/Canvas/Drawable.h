#pragma once
#include "Editor/Canvas/Image.h"
#include "Game/Core/Singleton.h"
#include "Game/Core/StringID.h"
#include <unordered_map>

class CImageResourceManager : public CSingleton<CImageResourceManager>
{
    typedef std::unordered_map<CStringID::IDType, CImage*> TImageResources;

public:
	void DestroyAllResources();
    CImage* GetImage( CStringID imageID );
    CImage* GetImage( const wxString& path );
    CImage* TryGetImage( wxString path );

private:
    TImageResources m_imageResources;
};

class CDrawable
{
public:
    float x,y, angle;
    float xscale, yscale;
    CImage* m_image;
    bool xflip, yflip;

    CDrawable( const wxString& path );
    CDrawable(CImage* image=(CImage*)0);
    void setFlip(bool x, bool y);
    void move(float x, float y);
    void scale(float x, float y);
    void scale(float k);
    void setImage(CImage* image);
    void render();
    void rotate(int angle);
    bool IsInside( double posx, double posy);
    void SetCenter( bool center);
    void setHotspot(float x, float y);

private:
    bool m_isInside;
    float hotspotX, hotspotY;
};