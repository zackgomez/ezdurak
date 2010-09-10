#include "GameOverState.h"
#include <sstream>
#include "GUIApp.h"
#include "QuitState.h"
#include "MenuState.h"

#include "core/Game.h"
#include "ai/AIPlayer.h"

using std::stringstream;

GUIStatePtr GameOverState::create(ConstPlayerPtr biscuit)
{
    GUIStatePtr ret(new GameOverState(biscuit));
    return ret;
}

GameOverState::GameOverState(ConstPlayerPtr biscuit)
{
    stringstream ss;
    if (biscuit.get())
        ss << biscuit->getName() << " is the biscuit!";
    else
        ss << "The game is a tie.";

    biscuitStr_ = GUIString::create(ss.str());
    instrStr_ = GUIString::create("Press any key to return to the menu, ESC to quit");
}

GameOverState::~GameOverState()
{ /* Empty */ }

void GameOverState::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(GUIApp::SCREENX/2, GUIApp::SCREENY/2, 0);

    glColor3f(1,1,1);
    biscuitStr_->draw();

    glTranslatef(0, GUIApp::SCREENY/5, 0);
    instrStr_->draw();
}

void GameOverState::processEvent(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
        next_ = QuitState::create();
    else if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
    {
        next_ = MenuState::create();
    }
}

bool GameOverState::needsTransition() const
{
    return next_.get() != NULL;
}

GUIStatePtr GameOverState::nextState()
{
    return next_;
}
