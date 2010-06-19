#include "GamePlayer.h"
#include <iostream>

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::set;

GamePlayer::GamePlayer(string& name, vector<Card>& hand)
    : hand_(hand), name_(name)
{}

GamePlayer::~GamePlayer()
{}

Card GamePlayer::defend(Card& attackingCard, Card::cardsuit trump)
{
    for (;;)
    {
        cout << "Defend: " << attackingCard << '\n';
        print();
        cout << "Enter card number, 0 to give up: ";
        int cnum;
        cin >> cnum;

        if (!cin)
        {
            cin.clear();
            cin.ignore(100, '\n');
            continue;
        }

        // Did they enter a valid index?
        if (cnum < 0 || cnum > hand_.size())
        {
            cout << "Enter a number between 0 and " << hand_.size() << '\n';
            continue;
        }

        // Did they give up?
        if (cnum == 0)
            return Card();

        // Map 1..n to 0..n-1
        cnum--;
        // Translate index to card
        Card attempt = hand_[cnum];

        // Validate attempt
        if (!attempt.beats(attackingCard, trump))
        {
            cout << "INVALID CARD\n";
            continue;
        }

        // Remove it from their hand
        std::swap(hand_[cnum], hand_[hand_.size()-1]);
        hand_.pop_back();

        return attempt;
    }
}

Card GamePlayer::attack(set<int> playableRanks)
{
    for (;;)
    {
        cout << "Attack:\n";
        print();
        cout << "Enter card number, 0 to pass: ";
        int cnum;
        cin >> cnum;

        if (!cin)
        {
            cin.clear();
            cin.ignore(100, '\n');
            continue;
        }

        // Did they enter a valid index?
        if (cnum < 0 || cnum > hand_.size())
        {
            cout << "Enter a number between 0 and " << hand_.size() << '\n';
            continue;
        }

        // Did they pass attacking?
        if (cnum == 0)
        {
            // You must attack if you are the first attacker
            if (playableRanks.empty())
                continue;
            return Card();
        }

        // Map 1..n to 0..n-1
        cnum--;
        // Translate index to card
        Card attempt = hand_[cnum];

        // Validate attempt
        if (!playableRanks.empty() &&
            !playableRanks.count(attempt.getNum()))
        {
            cout << "INVALID CARD\n";
            continue;
        }

        // Remove it from their hand
        std::swap(hand_[cnum], hand_[hand_.size()-1]);
        hand_.pop_back();

        return attempt;
    }
}

void GamePlayer::addCards(vector<Card>& cards)
{
    hand_.insert(hand_.end(), cards.begin(), cards.end());
}

int GamePlayer::numCards() const
{
    return hand_.size();
}

void GamePlayer::print() const
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
