#include "CLIListener.h"
#include <vector>
#include <iostream>
#include "core/Player.h"

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
    cout << "Game is starting.\n";
    cout << "Players are seated like:\n";
    const vector<PlayerPtr> players = agent_->getPlayers();
    for (int i = 0; i < players.size(); i++)
        cout << players[i]->getName() << " ";
    cout << '\n';
    cout << "Trump is " << agent_->getTrumpCard() << '\n';
}

void CLIListener::gameOver(const ConstPlayerPtr biscuitPlayer)
{
    if (biscuitPlayer)
        cout << "The biscuit is " << biscuitPlayer->getName() << '\n';
    else
        cout << "The game is a draw.\n";
}

void CLIListener::newRound(ConstPlayerPtr attacker, ConstPlayerPtr defender)
{
    attacker_ = attacker;
    defender_ = defender;

    cout << "---------------------------------------------------\n"
         << "Start of a new round\n"
         << "Attacker: " << attacker_->getName() << " - "
         << attacker_->getNumCards() << " cards in hand\n"
         << "Defender: " << defender_->getName() << " - "
         << defender_->getNumCards() << " cards in hand\n";
}

void CLIListener::attackerPassed(const ConstPlayerPtr attacker)
{
    cout << attacker_->getName() << " has passed.\n";
    attacker_ = agent_->getAttacker();
    cout << "The attacker is now " << attacker_->getName()
         << "(" << attacker_->getNumCards() << ")\n";
}

void CLIListener::endRound(bool successfulDefend)
{
    if (successfulDefend)
        cout << defender_->getName() << " has successfully defended!\n";
    else
        cout << defender_->getName() << " was unable to defend.  "
             << "Cards will now be piled on.\n";
}

void CLIListener::attackingCard(const Card &c)
{
    cout << attacker_->getName() << '(' << attacker_->getNumCards() << ")" 
         << " attacked with " << c << '\n';
}

void CLIListener::defendingCard(const Card &c)
{
    cout << defender_->getName() << '(' << defender_->getNumCards() << ")" 
         << " defended with " << c << '\n';
}

void CLIListener::piledOnCard(const Card &c)
{
    cout << c << " was piled on\n";
}

void CLIListener::playedOut(const ConstPlayerPtr player)
{
    cout << player->getName() << " has gone out!\n";
}

void CLIListener::givenCards(const ConstPlayerPtr player, int numCards)
{
    cout << player->getName() << " has been given " << numCards << " cards\n";
}
