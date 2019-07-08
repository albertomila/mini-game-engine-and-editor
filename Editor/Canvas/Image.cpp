#include "Editor_PCH.h"
#include "Editor/Canvas/Image.h"

#include "Editor/Canvas/ImageLoader.h"
#include <cmath>
#include "wx/image.h"
#include "wx/wx.h"

CImage::CImage()
{
}

CImage::CImage(wxString path)
{
    load(path);
}

void CImage::load(wxString path)
{
    ID = ImageLoader::LoadImage(path, &width, &height, &textureWidth, &textureHeight);

    tex_coord_x = (float)width/(float)textureWidth;
    tex_coord_y = (float)height/(float)textureHeight;
}

GLuint* CImage::getID()
{
    return ID;
}

CImage::~CImage()
{
    glDeleteTextures (1, ID);
}