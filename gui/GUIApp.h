#pragma once
#include "gl.h"
#include <string>
#include "GUIState.h"

class GUIApp
{
public:
	// Constants
	/// Screen size in the x dimension
	static const int SCREENX;
	/// Screen size in the y dimension
	static const int SCREENY;
    GUIApp();
    ~GUIApp();

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
