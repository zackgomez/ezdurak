#pragma once
#include "GUIState.h"

class QuitState :
    public GUIState
{
public:
    QuitState()  { /* Empty */ }
    ~QuitState() { /* Empty */ }

    void render() { /* Empty */}
    void processEvent(SDL_Event&) { /* Empty */}
    bool needsTransition() const { return true; }
    GUIStatePtr nextState() { return GUIStatePtr(); }
};
