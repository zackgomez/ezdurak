#pragma once
#include "GUIState.h"
#include "GUIString.h"
#include "core/Player.h"

/** 
 * This state displays the results of the last game and prompts for action.
 */
class GameOverState :
    public GUIState
{
public:
    static GUIStatePtr create(ConstPlayerPtr firstOut, ConstPlayerPtr biscuit);

    /// Destructor
    ~GameOverState();

    // Methods inherited from GUIState Interface
    void render();
    void processEvent(SDL_Event& e);
    bool needsTransition() const;
    GUIStatePtr nextState();

private:
    // Private constructors for create idiom
    GameOverState(ConstPlayerPtr firstOut, ConstPlayerPtr biscuit);
    GameOverState(const GameOverState&);
    void operator=(const GameOverState&);

    // Data members
    GUIStringPtr biscuitStr_;
    GUIStringPtr firstOutStr_;
    GUIStringPtr instrStr_;
    GUIStatePtr next_;
};
