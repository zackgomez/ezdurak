#pragma once
#include <pthread.h>
#include <GL/gl.h>
#include <SDL/SDL_ttf.h>
#include <vector>
#include <string>
#include "../core/Card.h"

class Player;
class GUIString;
class GUIPlayerView;
class GUIHumanView;

class GUIImpl
{
public:
    GUIImpl();
    ~GUIImpl();

    void run();

    void setPlayers(const std::vector<Player*>& players);
    void setAttacker(const Player* p);
    void setDefender(const Player* p);
    void setTrumpCard(const Card &c);
    void clearPlayedCards();
    void addAttackingCard(const Card& c);
    void addDefendingCard(const Card& c);
    void setPileSizes(int deckSize, int discardSize);

    void wait(int ms);

private:
    bool cont_;

    std::vector<Card> attackingCards_;
    std::vector<Card> defendingCards_;
    int deckSize_;
    int discardSize_;
    pthread_mutex_t playedCardsLock_;

    std::vector<Player*> players_;
    pthread_mutex_t playersLock_;

    bool validPlayerDisplays_;
    GUIHumanView *humanView_;
    std::vector<GUIPlayerView*> playersDisplay_;
    bool validStatus_;
    const Player *attacker_, *defender_;

    bool validSizes_;
    GUIString *deckString_;
    GUIString *discardString_;
    
    Card trumpCard_;

    // Helper functions
    void initGL();
    void processEvents();
    void render();
    void drawPlayedCards();
    void drawPiles();
    void drawPlayers();
    void updatePlayers();

    GLuint loadTexture(const std::string& filename);
};
