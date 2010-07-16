#pragma once
#include <pthread.h>
#include "gl.h"
#include <SDL/SDL_ttf.h>
#include <vector>
#include <string>
#include "core/Card.h"
#include "GUIString.h"
#include "SynchronizedQueue.h"
#include "core/Player.h"

class GUIPlayerView;
class GUIHumanView;
class GUIListener;
class Game;

class GUIImpl
{
public:
    GUIImpl();
    ~GUIImpl();

    void run();

    void setPlayers(const std::vector<PlayerPtr>& players);
    void setAttacker(ConstPlayerPtr p);
    void setDefender(ConstPlayerPtr p);
    void setTrumpCard(const Card &c);
    void clearPlayedCards();
    void addAttackingCard(const Card& c);
    void addDefendingCard(const Card& c);
    void setPileSizes(int deckSize, int discardSize);
    void setBiscuit(ConstPlayerPtr p);

    void wait(int ms);

private:
    bool cont_;

    std::vector<Card> attackingCards_;
    std::vector<Card> defendingCards_;
    int deckSize_;
    int discardSize_;
    pthread_mutex_t playedCardsLock_;

    std::vector<PlayerPtr> players_;
    pthread_mutex_t playersLock_;

    bool validPlayerDisplays_;
    GUIHumanView *humanView_;
    std::vector<GUIPlayerView*> playersDisplay_;
    bool validStatus_;
    ConstPlayerPtr attacker_, defender_;

    bool validSizes_;
    GUIStringPtr deckString_;
    GUIStringPtr discardString_;
    bool validBiscuit_;
    std::string biscuit_;
    GUIStringPtr biscuitStr_;
    
    Card trumpCard_;

    Game *game;
    GUIListener *listener;
    SynchronizedQueue<int> queue_;

    // Helper functions
    void initGL();
    void processEvents();
    void render();
    void drawPlayedCards();
    void drawPiles();
    void drawPlayers();
    void updatePlayers();

    void startGame(int numPlayers);

    GLuint loadTexture(const std::string& filename);
};
