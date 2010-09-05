#pragma once
#include "GUIState.h"
#include <string>
#include "gl.h"

class InitState :
    public GUIState
{
public:
    static GUIStatePtr create(GUIStatePtr nextState);

    virtual void render();
    virtual void processEvent(SDL_Event &e);
    virtual bool needsTransition() const;
    virtual GUIStatePtr nextState();

private:
    InitState(GUIStatePtr next);

    GUIStatePtr next_;

    // Helper Functions
    void initGL();
    void loadResources();
    GLuint loadTexture(const std::string &filename);
};

