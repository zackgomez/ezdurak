#include "GUIString.h"
#include <SDL/SDL_ttf.h>

TTF_Font * GUIString::font_ = NULL;

GUIStringPtr GUIString::create(const std::string &str)
{
    return GUIStringPtr(new GUIString(str));
}


GUIString::GUIString(const std::string &str) :
    tex_(0),
    width_(0),
    height_(0)
{
    SDL_Color color = {255,255,255};
    SDL_Surface *fontsurf = TTF_RenderText_Blended(font_, str.c_str(), color);

    if (!fontsurf)
        // Fail
        return;

    GLenum texture_format;
    if (fontsurf->format->BytesPerPixel == 4 )
        texture_format = (fontsurf->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
    else if (fontsurf->format->BytesPerPixel == 3 )
        texture_format = (fontsurf->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
    else
        // Fail
        return;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_RECTANGLE, texture);

    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 4, fontsurf->w, fontsurf->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, fontsurf->pixels);
    width_ = fontsurf->w;
    height_ = fontsurf->h;
    tex_ = texture;

    SDL_FreeSurface(fontsurf);
}

GUIString::~GUIString()
{
    glDeleteTextures(1, &tex_);
}

void GUIString::draw()
{
    glBindTexture(GL_TEXTURE_RECTANGLE, tex_);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);
        glVertex3f(-width_/2, -height_/2, 0);

        glTexCoord2i(0, height_);
        glVertex3f(-width_/2, height_/2, 0);

        glTexCoord2i(width_, height_);
        glVertex3f(width_/2, height_/2, 0);

        glTexCoord2i(width_, 0);
        glVertex3f(width_/2, -height_/2, 0);
    glEnd();
}

int GUIString::getHeight() const
{
    return height_;
}

int GUIString::getWidth() const
{
    return width_;
}
