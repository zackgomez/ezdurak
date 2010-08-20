#include "HumanCardHolder.h"
#include "GUIPlayer.h"
#include <iostream>
#include "core/Card.h"

HumanCardHolder::HumanCardHolder(const GUIPlayer *player, Card tc) :
    player_(player),
    cards_(player->getHand()),
    trumpCard_(tc)
{ /* Empty */ }

HumanCardHolder::~HumanCardHolder()
{ /* Empty */ }

void HumanCardHolder::addCard(Card c)
{
    // If the card is a Card(), then we need to add one from the player
    // that we don't already have
    if (!c)
    {
        std::vector<Card>::const_iterator pit = player_->getHand().begin();
        // Loop over the player cards
        for (; pit != player_->getHand().end(); pit++)
        {
            // If we don't already have that card
            if (find(cards_.begin(), cards_.end(), *pit) == cards_.end())
            {
                // Then that's our card
                c = *pit;
                break;
                std::cout << "Adding a " << c << " to human player.\n";
            }
        }
    }
            
    cards_.push_back(c);

    if (cards_.size() == player_->getNumCards())
        cards_ = player_->getHand();
    else
        // And sort so they stay synchronized with the GUIPlayer
        sort(cards_.begin(), cards_.end(), CardComp(trumpCard_.getSuit()));
}

void HumanCardHolder::addCards(const std::vector<Card>& cs)
{
    for (int i = 0; i < cs.size(); i++)
        addCard(cs[i]);
}

void HumanCardHolder::removeCard(Card c)
{
    // find the card
    std::vector<Card>::iterator it = std::find(cards_.begin(), cards_.end(),
                                               c);
    // Make sure we found a card
    assert( it != cards_.end() );
    cards_.erase(it);
}

void HumanCardHolder::clear()
{
    assert(false && "This function has no meaning for HumanCardHolder");
}

bool HumanCardHolder::contains(const Card &c) const
{
    return std::find(cards_.begin(), cards_.end(), c) != cards_.end();
}
