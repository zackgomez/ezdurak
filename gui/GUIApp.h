#pragma once
#include "gl.h"
#include <string>
#include "GUIState.h"

/**
 * This class is the "main" class for a GUI version of ezdurak.  It uses a
 * state based design that allows for much customization.
 */
class GUIApp
{
public:
    // Constants
    /// Screen size in the x dimension
    static const int SCREENX;
    /// Screen size in the y dimension
    static const int SCREENY;

    /// Constructor
    GUIApp();
    /// Destructor
    ~GUIApp();

    /**
     * Runs a GUI game.
     */
    void run();

private:
    /// Controls the main loop, true if it should continue
    bool cont_;
    /// Current game state
    GUIStatePtr state_;

    // Helper functions
    void initGL();
    void processEvents();
    void render();

    GLuint loadTexture(const std::string& filename);
};
