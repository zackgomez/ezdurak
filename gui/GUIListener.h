#pragma once
#include "core/GameListener.h"

class GUIImpl;

class GUIListener :
    public GameListener
{
public:
    GUIListener(GameAgent *agent, GUIImpl *impl);
    ~GUIListener();

    // Functions inherited from GameListener Interface
    void gameStart();
    void gameOver(ConstPlayerPtr biscuit);
    void newRound(ConstPlayerPtr attacker, ConstPlayerPtr defender);
    void attackerPassed(ConstPlayerPtr newAttacker);
    void endRound(bool successfulDefend);
    void attackingCard(const Card &c);
    void defendingCard(const Card &c);
    void piledOnCard(const Card &c);
    void playedOut(ConstPlayerPtr player);
    void givenCards(ConstPlayerPtr player, int numCard);

private:
    GUIImpl *impl_;
};

