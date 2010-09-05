#include "GUIApp.h"
#include <SDL/SDL.h>
#include <iostream>
#include "MenuState.h"
#include "InitState.h"

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
    state_ = InitState::create(MenuState::create());

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
