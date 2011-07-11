#include "core/GameListener.h"

/**
 * An implemenation of the GameListener interface that pritns relevantm messages
 * to stdout when an event happens.
 */
class CLIListener :
    public GameListener
{
public:
    CLIListener();
    virtual ~CLIListener();

    // Methods inherited from GameListener
    void gameStart(GameAgent *);
    void gameOver(ConstPlayerPtr, ConstPlayerPtr);
    void newRound(ConstPlayerPtr, ConstPlayerPtr);
    void endRound(bool);
    void attackerPassed(ConstPlayerPtr);
    void attackingCard(const Card &c);
    void defendingCard(const Card &c);
    void piledOnCard(const Card &c);
    void playedOut(ConstPlayerPtr player);
    void givenCards(ConstPlayerPtr player, int numCards);
    void givenCards(ConstPlayerPtr player, const std::vector<Card>& cards);
    
private:
    GameAgent *agent_;
    ConstPlayerPtr attacker_, defender_;
};
