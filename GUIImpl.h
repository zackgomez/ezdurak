#pragma once
#include <pthread.h>
#include <GL/gl.h>
#include <vector>
#include <string>
#include "Card.h"

class GUIImpl
{
public:
    GUIImpl();
    ~GUIImpl();

    void run();

    void setPlayedCards(const std::vector<Card>& newCards);

private:
    bool cont;
    std::vector<Card> playedCards;
    GLuint cardtex;

    // Helper functions
    void initGL();
    void render();
    void processEvents();
    void drawCard(int row, int col);
    GLuint loadTexture(const std::string& filename);
};
