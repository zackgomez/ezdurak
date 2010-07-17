#pragma once
#include "GUIState.h"

class QuitState :
    public GUIState
{
    QuitState()  { /* Empty */ }
    ~QuitState() { /* Empty */ }

    void render() { /* Empty */}
    void processEvent(SDL_Event&) { /* Empty */}
    GUIStatePtr nextState() { return GUIStatePtr(); }
}