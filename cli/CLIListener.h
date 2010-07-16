#include "core/GameListener.h"

class CLIListener : public GameListener
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
    ConstPlayerPtr attacker_, defender_;
};
