#include "GUIApp.h"
#include <SDL/SDL.h>
#include <iostream>
<<<<<<< HEAD
#include "MenuState.h"

#include "GUICard.h"
#include "GUIPlayerView.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
=======
#include "InitState.h"
#include "InGameState.h"
>>>>>>> master

#include "InGameState.h"
#include "core/Game.h"
#include "ai/AIPlayer.h"
#include <sstream>

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
<<<<<<< HEAD
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

    /*
=======
>>>>>>> master
    GamePtr game(new Game());
    std::stringstream ss;
    for (int i = 0; i < 3; i++)
    {
	ss.str("");
	ss << "AIPlayer" << i+1;
	PlayerPtr p(new AIPlayer(ss.str()));
	game->addPlayer(p);
    }
<<<<<<< HEAD
    state_ = InGameState::create(game);
    */
    state_ = MenuState::create();
=======
    state_ = InitState::create(InGameState::create(game));
>>>>>>> master

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
                assert(false && "NULL state returned");
        }

        SDL_Delay(16);
    }

    TTF_Quit();
    SDL_Quit();
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
