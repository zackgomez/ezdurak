#include "Game.h"
#include <cassert>
#include "Player.h"

using namespace std;

Game::Game(const std::vector<Player*>& players) :
    players_(players)
{
    assert(players_.size() >= 2 && players_.size() <= 6);

    attacker_ = players_[0];
    defender_ = players_[1];
}

Game::~Game()
{ /* Empty */ }

void Game::run()
{
    deal();

    while (players_.size() > 1)
    {
        // Round
        // Refill
        // Go out
        // new defender
    }
}

void Game::deal()
{
    deck_ = Deck();
    deck_.shuffle();
    // Bottom card is trump
    trumpCard_ = deck_.peekLast();

    // Deal each player a hand.
    for (int i = 0; i < players_.size(); i++)
    {
        std::vector<Card> hand = deck_.deal(HAND_SIZE);
        players_[i]->addCards(hand);
    }
}

// ------------------ GameAgent Interface -------------------------
void Game::addListener(GameListener *listener)
{
    listeners_.insert(listener);
}

void Game::removeListener(GameListener *listener)
{
    listeners_.erase(listener);
}

Card Game::getTrumpCard() const
{
    return trumpCard_;
}

int Game::getTricksLeft() const
{
    // TODO
    return -1;
}

int Game::getDeckSize() const
{ 
    return deck_.getNumCards();
}

const Player * Game::getAttacker() const
{
    return attacker_;
}

const Player * Game::getDefender() const
{
    return defender_;
}

const std::vector<Player*>& Game::getPlayers() const
{
    return players_;
}

const std::vector<Card>& Game::getPlayedCards() const
{
    return playedCards_;
}
