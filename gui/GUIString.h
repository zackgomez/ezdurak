#pragma once
#include <string>
#include "gl.h"
#include <SDL/SDL_ttf.h>
#include <memory>

class GUIString
{
public:
    GUIString(const std::string &str);
    ~GUIString();

    static TTF_Font *font_;

    void draw();

private:
    GLuint tex_;
    int width_, height_;
};

typedef std::auto_ptr<GUIString> GUIStringPtr;
