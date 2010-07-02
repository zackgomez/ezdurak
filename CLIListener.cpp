#include "CLIListener.h"
#include <vector>
#include <iostream>
#include "Player.h"

using std::vector;
using std::cout;

CLIListener::CLIListener(GameAgent *agent) :
    GameListener()
{
    agent_ = agent;
    agent_->addListener(this);
}

CLIListener::~CLIListener()
{ /* Empty */ }

void CLIListener::gameStart()
{
}

void CLIListener::gameOver(const Player* biscuitPlayer)
{
    if (biscuitPlayer)
        cout << "The biscuit is " << biscuitPlayer->getName() << '\n';
    else
        cout << "The game is a draw.\n";
}

void CLIListener::newRound(const Player* attacker, const Player* defender)
{
    attacker_ = attacker;
    defender_ = defender;

    cout << "---------------------------------------------------\n"
         << "Start of a new round\n"
         << "Attacker: " << attacker_->getName() << '\n'
         << "Defender: " << defender_->getName() << "\n\n";
}

void CLIListener::attackerChanged(const Player *attacker)
{
    attacker_ = attacker;
}

void CLIListener::endRound(bool successfulDefend)
{
    if (successfulDefend)
        cout << defender_->getName() << " has successfully defended!\n";
    else
        cout << defender_->getName() << " was unable to defend.\n";
}

void CLIListener::attackingCard(const Card &c)
{
    cout << attacker_->getName() << " attacked with " << c << '\n';
}

void CLIListener::defendingCard(const Card &c)
{
    cout << defender_->getName() << " defended with " << c << '\n';
}

void CLIListener::piledOnCard(const Card &c)
{
    cout << c << " was piled on\n";
}

void CLIListener::playedOut(const Player *player)
{
    cout << player->getName() << " has gone out!\n";
}

void CLIListener::givenCards(const Player *player, int numCards)
{
}
