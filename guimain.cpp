#include "GUIListener.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <string>
#include <iostream>
#include <cassert>

using namespace std;

GLuint loadTexture(string filename);
void drawCard(int row, int col);

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);

    GLuint cardtex = loadTexture("resources/cards3.png");

    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_RECTANGLE);
    glBindTexture(GL_TEXTURE_RECTANGLE, cardtex);

    glScalef(100, 155, 0);
    drawCard(12,2);

    SDL_GL_SwapBuffers();

    SDL_Event e;
    bool cont = true;
    while (cont)
    {
        while (SDL_PollEvent(&e))
        {
            switch(e.type)
            {
            case SDL_QUIT:
                cont = false; break;
            case SDL_KEYDOWN:
                cont = false; break;
            }
        }
    }

    SDL_Quit();
    return 0;
}

GLuint loadTexture(string filename)
{
    SDL_Surface *tex;
    tex = IMG_Load(filename.c_str());

    if (!tex)
    {
        cout << "Unable to load image" << IMG_GetError() << '\n';
        exit(1);
    }

    GLenum texture_format;
    if ( tex->format->BytesPerPixel == 4 )
        texture_format = (tex->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
    else if (tex->format->BytesPerPixel == 3 )
        texture_format = (tex->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
    else
    {
        cout << "BPP: " << tex->format->BytesPerPixel << '\n';
        assert(false && "Image is not in the proper format.");
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_RECTANGLE, texture);

    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 4, tex->w, tex->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, tex->pixels);

    SDL_FreeSurface(tex);

    return texture;
}

void drawCard(int row, int col)
{
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2i(row * 79, col * 123);
        glVertex3f(0, 0, 0);

        glTexCoord2i(row * 79, (col+1) * 123);
        glVertex3f(0, 1, 0);

        glTexCoord2i((row+1) * 79, (col+1) * 123);
        glVertex3f(1, 1, 0);

        glTexCoord2i((row+1) * 79, col * 123);
        glVertex3f(1, 0, 0);
    glEnd();
}
