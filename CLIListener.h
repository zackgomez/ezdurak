#include "GameListener.h"

class CLIListener : public GameListener
{
public:
    CLIListener(GameAgent *agent);
    virtual ~CLIListener();

    // Methods inherited from GameListener
    void gameStart();
    void gameOver(const Player*);
    void attackerChanged(const Player*);
    void defenderChanged(const Player*);
    void defenderLost();
    void defenderWon();
    void attackingCard(const Card &c);
    void defendingCard(const Card &c);
    void playedOut(const Player *player);
    void givenCards(const Player *player, int numCards);
    
private:
    const Player *attacker_, *defender_;
};
