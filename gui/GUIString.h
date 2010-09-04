#pragma once
#include <string>
#include "gl.h"
#include <SDL/SDL_ttf.h>
#include <boost/shared_ptr.hpp>

// Forward declaration of GUIStringPtr
class GUIString;
typedef boost::shared_ptr<GUIString> GUIStringPtr;

/** 
 * View for a string in the GUI.  Has methods for displaying a string to the
 * screen.
 */
class GUIString
{
public:
    static GUIStringPtr create(const std::string &str);
    ~GUIString();

    /** 
     * The font used to draw all strings.
     */
    static TTF_Font *font_;

    /** 
     * Draw the string at the current location (centered).
     */
    void draw();
    /** 
     * @return The height of the string in pixels.
     */
    int getHeight() const;
    /** 
     * @return The width of the string in pixels.
     */
    int getWidth() const;

private:
    GUIString(const std::string &str);

    GLuint tex_;
    int width_, height_;
};
