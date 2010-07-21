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
    bool needsTransition() const;
    GUIStatePtr nextState();

private:
    GUIStringPtr biscuitStr_;
    GUIStringPtr instrStr_;
    GUIStatePtr next_;
};
