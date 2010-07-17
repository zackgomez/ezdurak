#pragma once
#include "GUIState.h"
#include "GUIString.h"
#include "core/Player.h"

class GameOverState :
    public GUIState
{
public:
    GameOverState(ConstPlayerPtr biscuit);
    ~GameOverState();

    void render();
    void processEvent(SDL_Event& e);
    GUIStatePtr nextState();

private:
    GUIStringPtr str_;
    GUIStatePtr next_;
};