#include "CLIPlayer.h"
#include <iostream>
#include <algorithm>
#include "../core/GameAgent.h"

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::set;
using std::sort;

CLIPlayer::CLIPlayer(const string& name)
    : Player(name)
{}

CLIPlayer::~CLIPlayer()
{}

void CLIPlayer::gameStarting(GameAgent *agent)
{
    agent_ = agent;
}

Card CLIPlayer::defend(const Card& attackingCard, Card::cardsuit trump)
{
    sortHand();
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

Card CLIPlayer::attack(set<int> playableRanks)
{
    sortHand();
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

Card CLIPlayer::pileOn(std::set<int> playableRanks)
{
    cout << "Piling cards on\n";
    return attack(playableRanks);
}

void CLIPlayer::addCards(const std::vector<Card>& cards)
{
    cout << name_ << ":\n";
    cout << "Added cards:\n";

    for (int i = 0; i < cards.size(); i++)
        cout << cards[i] << ' ';
    cout << '\n';

    Player::addCards(cards);
}

class CardComp
{
private:
    Card::cardsuit trump;
public:
    CardComp(Card::cardsuit trumpsuit) :
        trump(trumpsuit)
    {}

    bool operator()(const Card &a, const Card &b)
    {
        Card::cardsuit aSuit = a.getSuit();
        Card::cardsuit bSuit = b.getSuit();

        // Case 1: one trump, one not trump
        if ((aSuit == trump && bSuit != trump) || 
            (bSuit == trump && aSuit != trump))
        {
            return a.getSuit() != trump;
        }
        // Case 2: both trump or both not trump
        return a.getNum() < b.getNum();
    }
};

void CLIPlayer::sortHand()
{
    Card::cardsuit trump = agent_->getTrumpCard().getSuit();
    sort(hand_.begin(), hand_.end(), CardComp(trump));
}
