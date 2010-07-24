#pragma once
#include <string>
#include "gl.h"
#include <SDL/SDL_ttf.h>
#include <boost/shared_ptr.hpp>

// Forward declaration of GUIStringPtr
class GUIString;
typedef boost::shared_ptr<GUIString> GUIStringPtr;

class GUIString
{
public:
    static GUIStringPtr create(const std::string &str);
    ~GUIString();

    static TTF_Font *font_;

    void draw();
    int getHeight() const;
    int getWidth() const;

private:
    GUIString(const std::string &str);

    GLuint tex_;
    int width_, height_;
};
