#pragma once
#include <string>
#include <GL/gl.h>
#include <SDL/SDL_ttf.h>

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

