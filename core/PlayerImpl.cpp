#include "PlayerImpl.h"
#include <iostream>

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::set;

PlayerImpl::PlayerImpl(const string& name)
    : hand_(), name_(name)
{}

PlayerImpl::~PlayerImpl()
{}

void PlayerImpl::addCards(const vector<Card>& cards)
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
    cout << name_ << ":\n";
    // Print numbers
    for (int i = 0; i < hand_.size(); i++)
    {
        cout << ' ' << i + 1;
        if (i + 1 < 10)
            cout << "  ";
        else
            cout << ' ';
    }
    cout << '\n';

    // Print cards
    for (int i = 0; i < hand_.size(); i++)
        cout << hand_[i] << ' ';
    cout << '\n';
}
