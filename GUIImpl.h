#pragma once
#include <pthread.h>
#include <GL/gl.h>
#include <vector>
#include <string>
#include "Card.h"

class Player;

class GUIImpl
{
public:
    GUIImpl();
    ~GUIImpl();

    void run();

    void setPlayers(const std::vector<Player*>& players);
    void setPlayedCards(const std::vector<Card>& newCards);

    void wait(int ms);

private:
    bool cont_;
    GLuint cardtex_;
    std::vector<Card> playedCards_;
    pthread_mutex_t playedCardsLock_;
    std::vector<Player*> players_;
    pthread_mutex_t playersLock_;

    // Helper functions
    void initGL();
    void render();
    void processEvents();
    void drawCard(int row, int col);
    GLuint loadTexture(const std::string& filename);
};
