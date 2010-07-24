#include "GUIApp.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include "InGameState.h"
#include "GUICard.h"

using namespace std;

const int GUIApp::SCREENX = 800;
const int GUIApp::SCREENY = 600;
#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

GUIApp::GUIApp()
{
}

GUIApp::~GUIApp()
{
    SDL_Quit();
}

void GUIApp::run()
{
    initGL();

    if (TTF_Init())
    {
        cout << "TTF_Init: " << TTF_GetError() << '\n';
        exit(2);
    }
#ifndef MAC_OSX
    GUIString::font_ = TTF_OpenFont("resources/FreeMonoBold.ttf", 16);
#else
    GUIString::font_ = TTF_OpenFont("../Resources/FreeMonoBold.ttf", 16);
#endif
    if (!GUIString::font_)
    {
        cout << "Unable to open font: " << TTF_GetError() << '\n';
        exit(3);
    }

    state_ = InGameState::create(4);

    cont_ = true;

    while (cont_)
    {
        render();

        processEvents();

        if (state_->needsTransition())
        {
            GUIStatePtr next = state_->nextState();
            if (next.get())
                state_ = next;
            else
                cont_ = false;
        }

        SDL_Delay(16);
    }

    TTF_Quit();
    SDL_Quit();
}

void GUIApp::initGL()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_SetVideoMode(SCREENX, SCREENY, 32, SDL_OPENGL);

    SDL_WM_SetCaption("EZDurak", "EZDurak");

    glEnable(GL_TEXTURE_RECTANGLE);
#ifndef MAC_OSX
    GUICard::cardtex = loadTexture("resources/cards.png");
#else
    GUICard::cardtex = loadTexture("cards.png");
#endif

    glViewport(0, 0, SCREENX, SCREENY);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, SCREENX, SCREENY, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0, 0.5, 0, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // TODO Fix this
    // Make the cards bigger
    GUICard::CARDX *= 1.2;
    GUICard::CARDY *= 1.2;
}

void GUIApp::processEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            cont_ = false;
        else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)
            cont_ = false;
        else
            state_->processEvent(e);
    }
}

void GUIApp::render()
{
    state_->render();

    SDL_GL_SwapBuffers();
}

GLuint GUIApp::loadTexture(const string& filename)
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
