#include "PlayerImpl.h"
#include <iostream>

PlayerImpl::PlayerImpl(const std::string& name)
    : Player(), hand_(), name_(name), agent_(NULL)
{}

PlayerImpl::~PlayerImpl()
{}

void PlayerImpl::gameStarting(GameAgent *agent)
{
    agent_ = agent;
}

void PlayerImpl::addCards(const std::vector<Card>& cards)
{
    hand_.insert(hand_.end(), cards.begin(), cards.end());
}

int PlayerImpl::getNumCards() const
{
    return hand_.size();
}

std::string PlayerImpl::getName() const
{
    return name_;
}

void PlayerImpl::print() const
{
    std::cout << name_ << ":\n";
    // Print numbers
    for (int i = 0; i < hand_.size(); i++)
    {
        std::cout << ' ' << i + 1;
        if (i + 1 < 10)
            std::cout << "  ";
        else
            std::cout << ' ';
    }
    std::cout << '\n';

    // Print cards
    for (int i = 0; i < hand_.size(); i++)
        std::cout << hand_[i] << ' ';
    std::cout << '\n';
}
