#include "core/GameListener.h"

class CLIListener : public GameListener
{
public:
    CLIListener(GameAgent *);
    virtual ~CLIListener();

    // Methods inherited from GameListener
    void gameStart();
    void gameOver(const Player*);
    void newRound(const Player*, const Player*);
    void endRound(bool);
    void attackerPassed(const Player*);
    void attackingCard(const Card &c);
    void defendingCard(const Card &c);
    void piledOnCard(const Card &c);
    void playedOut(const Player *player);
    void givenCards(const Player *player, int numCards);
    
private:
    const Player *attacker_, *defender_;
};
