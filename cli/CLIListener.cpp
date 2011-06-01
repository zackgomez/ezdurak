#include "CLIListener.h"
#include <vector>
#include <iostream>
#include "core/Player.h"
#include "core/GameAgent.h"
#include <boost/weak_ptr.hpp>

using std::vector;
using std::cout;

CLIListener::CLIListener() :
    GameListener()
{
}

CLIListener::~CLIListener()
{ /* Empty */ }

void CLIListener::gameStart(GameAgent *agent)
{
    agent_ = agent;
    cout << "Game is starting.\n";
    cout << "Players are seated like:\n";
    const vector<PlayerPtr> players = agent_->getPlayers();
    for (unsigned i = 0; i < players.size(); i++)
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

void CLIListener::deflectedCard(const Card &c, ConstPlayerPtr newAttacker,
        ConstPlayerPtr newDefender)
{
    cout << defender_->getName() << " deflected with " << c << '\n';
    attacker_ = newAttacker;
    defender_ = newDefender;
    cout << "Attacker: " << attacker_->getName() << "\nDefender: " <<
        defender_->getName() << '\n';
}

void CLIListener::playedOut(const ConstPlayerPtr player)
{
    cout << player->getName() << " has gone out!\n";
}

void CLIListener::givenCards(const ConstPlayerPtr player, int numCards)
{
    cout << player->getName() << " has been given " << numCards << " cards\n";
}

void CLIListener::givenCards(const ConstPlayerPtr player, const std::vector<Card>& cards)
{
    cout << player->getName() << " has been given the following cards:\n";
    for (unsigned i = 0; i < cards.size(); i++)
        cout << cards[i] << ' ';
    cout << '\n';
}
