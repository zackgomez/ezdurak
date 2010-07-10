#pragma once
#include <pthread.h>
#include <GL/gl.h>
#include <SDL/SDL_ttf.h>
#include <vector>
#include <string>
#include "Card.h"

class Player;
class GUIString;
class GUIPlayer;

class GUIImpl
{
public:
    GUIImpl();
    ~GUIImpl();

    void run();

    void setPlayers(const std::vector<Player*>& players);
    void clearPlayedCards();
    void addAttackingCard(const Card& c);
    void addDefendingCard(const Card& c);
    void setPileSizes(int deckSize, int discardSize);

    void wait(int ms);

private:
    bool cont_;
    GLuint cardtex_;

    std::vector<Card> attackingCards_;
    std::vector<Card> defendingCards_;
    int deckSize_;
    int discardSize_;
    pthread_mutex_t playedCardsLock_;

    std::vector<Player*> players_;
    pthread_mutex_t playersLock_;

    bool badPlayers_;
    std::vector<GUIPlayer*> playersDisplay_;

    bool validSizes_;
    GUIString *deckString_;
    GUIString *discardString_;

    // Helper functions
    void initGL();
    void render();
    void processEvents();
    void drawCard(int row, int col);
    GLuint loadTexture(const std::string& filename);
    void makeStringTexture(int i, const std::string& str);
};
