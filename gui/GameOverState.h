#pragma once
#include "GUIState.h"
#include "GUIString.h"
#include "core/Player.h"

class GameOverState :
    public GUIState
{
public:
    static GUIStatePtr create(ConstPlayerPtr biscuit);

    /// Destructor
    ~GameOverState();

    // Methods inherited from GUIState Interface
    void render();
    void processEvent(SDL_Event& e);
    bool needsTransition() const;
    GUIStatePtr nextState();

private:
    // Private constructors for create idiom
    GameOverState(ConstPlayerPtr biscuit);
    GameOverState(const GameOverState&);
    void operator=(const GameOverState&);

    // Data members
    GUIStringPtr biscuitStr_;
    GUIStringPtr instrStr_;
    GUIStatePtr next_;
};
