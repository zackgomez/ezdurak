#include "PlayerImpl.h"
#include <iostream>
#include <sstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

PlayerImpl::PlayerImpl(const std::string& name, const std::string &ID)
    : Player(), hand_(), name_(name), ID_(ID), agent_(NULL)
{
    if (ID_.empty())
        ID_ = getUniqueID();
}

PlayerImpl::~PlayerImpl()
{}

std::string PlayerImpl::getUniqueID()
{
    boost::uuids::uuid id = boost::uuids::random_generator()();
    std::stringstream ss;
    ss << '#' << id;
    return ss.str();
}

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

std::string PlayerImpl::getID() const
{
    return ID_;
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
