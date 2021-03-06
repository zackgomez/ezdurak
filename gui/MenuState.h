#pragma once
#include "GUIState.h"
#include "core/Game.h"
#include "GUIString.h"
#include "util/Logger.h"

class NetworkHost;
class NetworkClient;

class MenuState :
    public GUIState
{
public:
    static GUIStatePtr create();
    virtual ~MenuState();

    // Methods inherited from GUIState
    virtual void render();
    virtual void processEvent(SDL_Event& e);
    virtual bool needsTransition() const;
    virtual GUIStatePtr nextState();

private:
    MenuState();

    std::auto_ptr<NetworkHost> host_;
    std::auto_ptr<NetworkClient> client_;
    GamePtr game_;

    GUIStringPtr joinstr_, hoststr_, singlestr_, quitstr_;
    GUIStringPtr statusstr_;
    bool ready_;

    GUIStatePtr next_;

    LoggerPtr logger_;
};

