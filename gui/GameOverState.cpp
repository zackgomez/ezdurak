#include "GameOverState.h"
#include <sstream>
#include "GUIApp.h"
#include "InGameState.h"

using std::stringstream;

GameOverState::GameOverState(ConstPlayerPtr biscuit)
{
    stringstream ss;
    if (biscuit.get())
        ss << biscuit->getName() << " is the biscuit!";
    else
        ss << "The game is a tie.";

    str_ = GUIStringPtr(new GUIString(ss.str()));
}

GameOverState::~GameOverState()
{ /* Empty */ }

void GameOverState::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(GUIApp::SCREENX/2, GUIApp::SCREENY/2, 0);

    glColor3f(1,1,1);
    str_->draw();
}

void GameOverState::processEvent(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
        next_ = GUIStatePtr(new InGameState(4));
}

GUIStatePtr GameOverState::nextState()
{
    return next_;
}