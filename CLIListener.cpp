#include "CLIListener.h"
#include <vector>
#include <iostream>
#include "Player.h"

using std::vector;
using std::cout;

CLIListener::CLIListener() :
    GameListener()
{
}

CLIListener::~CLIListener()
{ /* Empty */ }

void CLIListener::gameStart(GameAgent* agent)
{
    GameListener::gameStart(agent);

    cout << "The players are seated like:\n";
    const vector<Player*> players = agent_->getPlayers();
    for (auto it = players.begin(); it != players.end(); it++)
        cout << (*it)->getName() << " ";
    cout << '\n';

    cout << agent_->getTrumpCard() << " is trump.\n";
}

void CLIListener::gameOver(const Player* biscuitPlayer)
{
    cout << "Game over.  ";
    if (biscuitPlayer)
        cout << "The biscuit is " << biscuitPlayer->getName() << '\n';
    else
        cout << "The game was a tie.\n";
}

void CLIListener::attackerChanged(const Player *newAttacker)
{
    attacker_ = newAttacker;
    cout << "The attacker is now " << attacker_->getName() << '\n';
}

void CLIListener::defenderChanged(const Player *newDefender)
{
    defender_ = newDefender;
    cout << "The defender is now " << defender_->getName() << '\n';
}

void CLIListener::defenderLost()
{
    cout << "The defender " << defender_->getName() << " has lost\n";
}

void CLIListener::defenderWon()
{
    cout << "The defender " << defender_->getName() << " has successfully"
        " defended\n";
}

void CLIListener::attackingCard(const Card &c)
{
    cout << attacker_->getName() << " played " << c << '\n';
}

void CLIListener::defendingCard(const Card &c)
{
    cout << defender_->getName() << " defended with " << c << '\n';
}

void CLIListener::playedOut(const Player *player)
{
    cout << player->getName() << " has gone out\n";
    cout << "The remaining players are seated like:\n";
    const vector<Player*> players = agent_->getPlayers();
    for (auto it = players.begin(); it != players.end(); it++)
        cout << (*it)->getName() << " ";
    cout << '\n';
}

void CLIListener::givenCards(const Player *player, int numCards)
{
    cout << player->getName() << " has been given " << numCards << " cards.\n";
}
