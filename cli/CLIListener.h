#include "core/GameListener.h"

/**
 * An implemenation of the GameListener interface that pritns relevantm messages
 * to stdout when an event happens.
 */
class CLIListener :
    public GameListener
{
public:
    CLIListener(GameAgent *);
    virtual ~CLIListener();

    // Methods inherited from GameListener
    void gameStart();
    void gameOver(ConstPlayerPtr);
    void newRound(ConstPlayerPtr, ConstPlayerPtr);
    void endRound(bool);
    void attackerPassed(ConstPlayerPtr);
    void attackingCard(const Card &c);
    void defendingCard(const Card &c);
    void piledOnCard(const Card &c);
    void playedOut(ConstPlayerPtr player);
    void givenCards(ConstPlayerPtr player, int numCards);
    
private:
    GameAgent *agent_;
    ConstPlayerPtr attacker_, defender_;
};
