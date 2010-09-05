#pragma once
#include "GUIState.h"
#include "GUICard.h"
#include "GUIPlayerView.h"

/** 
 * The GUI should be put to this state when the application should exit. Cleans 
 * up and exits the application.
 */
class QuitState :
    public GUIState
{
public:
    static GUIStatePtr create() {GUIStatePtr ret(new QuitState()); return ret;}

    /// Destructor
    ~QuitState() { /* Empty */ }

    // Methods inherited from GUIState Interface
    /** 
     * Cleans up resources.
     */
    void render()
    {
        // Free fonts
        TTF_CloseFont(GUIString::font_);

        // Free opengl textures
        glDeleteTextures(1, &GUICard::cardtex);
        glDeleteTextures(1, &GUIPlayerView::attackEmblem);
        glDeleteTextures(1, &GUIPlayerView::defendEmblem);
        glDeleteTextures(1, &GUIPlayerView::defendLostEmblem);

        TTF_Quit();
        SDL_Quit();

        exit(0);
    }
    void processEvent(SDL_Event&) { /* Empty */}
    bool needsTransition() const { assert(false); }
    GUIStatePtr nextState() { assert(false); return GUIStatePtr(); }

private:
    // Private constructors for create idiom
    QuitState()  { /* Empty */ }
    QuitState(const QuitState&);
    void operator=(const QuitState&);
};
