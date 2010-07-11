#pragma once
#include "../core/GameListener.h"

class GUIImpl;

class GUIListener :
    public GameListener
{
public:
    GUIListener(GameAgent *agent, GUIImpl *impl);
    ~GUIListener();

    // Functions inherited from GameListener Interface
    void gameStart();
    void gameOver(const Player* biscuit);
    void newRound(const Player* attacker, const Player* defender);
    void attackerPassed(const Player* newAttacker);
    void endRound(bool successfulDefend);
    void attackingCard(const Card &c);
    void defendingCard(const Card &c);
    void piledOnCard(const Card &c);
    void playedOut(const Player *player);
    void givenCards(const Player *player, int numCard);

private:
    GUIImpl *impl_;
};

