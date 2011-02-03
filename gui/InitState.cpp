#include "InitState.h"
#include <cassert>
#include <iostream>
#include "GUICard.h"
#include "GUIPlayerView.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "GUIApp.h"
#include "net/kissnet.h"

using namespace std;

GUIStatePtr InitState::create(GUIStatePtr nextState)
{
    GUIStatePtr ret(new InitState(nextState));
    return ret;
}

InitState::InitState(GUIStatePtr next) :
    next_(next)
{
    assert(next_.get());
}

void InitState::render()
{
    initGL();

    if (TTF_Init())
    {
        cerr << "TTF_Init: " << TTF_GetError() << '\n';
        exit(2);
    }
#ifndef MAC_OSX
    GUIString::font_ = TTF_OpenFont("resources/FreeMonoBold.ttf", 16);
#else
    GUIString::font_ = TTF_OpenFont("../Resources/FreeMonoBold.ttf", 16);
#endif
    if (!GUIString::font_)
    {
        cerr << "Unable to open font: " << TTF_GetError() << '\n';
        exit(3);
    }

    kissnet::init_networking();
}

void InitState::processEvent(SDL_Event &e)
{
}

bool InitState::needsTransition() const
{
    return true;
}

GUIStatePtr InitState::nextState()
{
    return next_;
}

void InitState::initGL()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_SetVideoMode(GUIApp::SCREENX, GUIApp::SCREENY, 32, SDL_OPENGL);

    SDL_WM_SetCaption("EZDurak", "EZDurak");

    glEnable(GL_TEXTURE_RECTANGLE);
#ifndef MAC_OSX
    GUICard::cardtex = loadTexture("resources/cards.png");
    GUIPlayerView::attackEmblem = loadTexture("resources/sword.png");
    GUIPlayerView::defendEmblem = loadTexture("resources/shield.png");
    GUIPlayerView::defendLostEmblem = loadTexture("resources/shieldx.png");
#else
    GUICard::cardtex = loadTexture("cards.png");
    GUIPlayerView::attackEmblem = loadTexture("sword.png");
    GUIPlayerView::defendEmblem = loadTexture("shield.png");
    GUIPlayerView::defendLostEmblem = loadTexture("shieldx.png");
#endif

    glViewport(0, 0, GUIApp::SCREENX, GUIApp::SCREENY);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, GUIApp::SCREENX, GUIApp::SCREENY, 0, -1, 1);
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


GLuint InitState::loadTexture(const string& filename)
{
    SDL_Surface *tex;
    tex = IMG_Load(filename.c_str());

    if (!tex)
    {
        cerr << "Unable to load image" << IMG_GetError() << '\n';
        exit(1);
    }

    GLenum texture_format;
    if ( tex->format->BytesPerPixel == 4 )
        texture_format = (tex->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
    else if (tex->format->BytesPerPixel == 3 )
        texture_format = (tex->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
    else
    {
        cerr << "BPP: " << tex->format->BytesPerPixel << '\n';
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
