#pragma once
#include "core/Card.h"
#include <vector>

/** 
 * Holds Cards in the GUI.  Is responsible for a set of cards and maintains
 * control over them.
 */
class CardHolder
{
public:
    virtual ~CardHolder() { /* Empty */ }

    /** 
     * Add a card.
     * 
     * @param c A card to add.
     */
    virtual void addCard(Card c) = 0;
    /** 
     * Adds a list of cards.
     * 
     * @param cs The cards to add.
     */
    virtual void addCards(const std::vector<Card>& cs) = 0;
    /** 
     * Removes the given card.
     * 
     * @param c A card to remove
     */
    virtual void removeCard(Card c) = 0;
    /** 
     * Removes all cards.
     */
    virtual void clear() = 0;
    /** 
     * Test if this CardHolder has a card.
     * 
     * @param c The card to check for.
     * 
     * @return True if this CardHolder contaisn the passed card.
     */
    virtual bool contains(const Card &c) const = 0;
};

