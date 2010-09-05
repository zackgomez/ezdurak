#pragma once
#include "GUIState.h"
#include "core/Game.h"
#include "GUIString.h"

class NetworkHost;
class NetworkClient;

class LANLobbyState :
    public GUIState
{
public:
    static GUIStatePtr create();
    virtual ~LANLobbyState();

    // Methods inherited from GUIState
    virtual void render();
    virtual void processEvent(SDL_Event& e);
    virtual bool needsTransition() const;
    virtual GUIStatePtr nextState();

private:
    LANLobbyState();

    std::auto_ptr<NetworkHost> host_;
    std::auto_ptr<NetworkClient> client_;
    GamePtr game_;

    GUIStringPtr joinstr_;
    GUIStringPtr hoststr_;

    GUIStatePtr next_;
};

