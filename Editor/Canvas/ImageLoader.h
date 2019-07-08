#pragma once

#ifdef __WXMAC__
#include "OpenGL/gl.h"
#else
#include <windows.h>
#include <GL/gl.h>
#endif
#include "wx/wx.h"

namespace ImageLoader
{
    GLuint* LoadImage(wxString path, int* imageWidth, int* imageHeight, int* textureWidth, int* textureHeight);
}