#pragma once
#include "GUIState.h"

class QuitState :
    public GUIState
{
public:
    static GUIStatePtr create() {GUIStatePtr ret(new QuitState()); return ret;}

    /// Destructor
    ~QuitState() { /* Empty */ }

    // Methods inherited from GUIState Interface
    void render() { /* Empty */}
    void processEvent(SDL_Event&) { /* Empty */}
    bool needsTransition() const { return true; }
    GUIStatePtr nextState() { return GUIStatePtr(); }

private:
    // Private constructors for create idiom
    QuitState()  { /* Empty */ }
    QuitState(const QuitState&);
    void operator=(const QuitState&);
};
