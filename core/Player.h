#pragma once
#include <vector>
#include <string>
#include <set>
#include <boost/shared_ptr.hpp>
#include "Card.h"

class GameAgent;

/** 
 * Interface that a class must implement to play a game in the Game class.
 * Contains methods for each action a player must take, attacking defending 
 * and drawing cards.  It also contains some getters for information about the
 * player like a name, ID or number of cards in hand.
 */
class Player
{
public:
    Player() {}
    virtual ~Player() {}

    /**
     * Called at the start of the game.
     */
    virtual void gameStarting(GameAgent *agent) = 0;

    /**
     * When a player is required to defend a card this method is called.  A
     * defending card must follow suit of the attackingCard and be a higher
     * rank or be trump.  If a defender cannot or does not wish to beat the
     * card Card() should be returned.
     * @param attackingCard The card the defender must beat.
     * @param trump The trump suit.
     * @return The defending card, or Card() if the defender passes.
     */
    virtual Card defend(const Card& attackingCard, Card::cardsuit trump) = 0;

    /**
     * When an attacking player has a chance to play a card this method is 
     * called.  An attacking card must have a rank in the playableRanks set.
     * If an attacker does not wish to play a card, Card() should be returned.
     * @param playableRanks A set of the playable ranks.
     * @return The attackers play or Card() if they pass.
     */
    virtual Card attack(std::set<int> playableRanks) = 0;

    /**
     * When the deflection variant is played, this function will be called if
     * there is a chance to deflect.  A defender wishing to deflect should
     * return a card of the appropriate rank to defend, or Card() if they do
     * not wish to deflect.  If they do not deflect, they will then have to
     * defend.
     * @param attackingCard One of the cards to deflect
     * @return A card of the same rank as attackingCard, or Card()
     */
    virtual Card deflect(const Card& attackingCard) = 0;

    /**
     * After a defender has given up, attackers may continue to give him cards.
     * This method is called when an attacker has a chance to pileOn cards to a
     * defender after he has given up.  Card() should be returned if the
     * attacker does not wish to play a card.
     * @param playableRanks The possible ranks for a piledOn card.
     * @return The piledOn card or Card() for a pass.
     */
    virtual Card pileOn(std::set<int> playableRanks) = 0;

    /**
     * This method is invoked when a player recieves cards.  This can happen for
     * one of two reasons.  The player may have unsuccessfully defended and 
     * these were the played cards, or the player may have required cards to
     * "fill up" at the end of the round.
     * @param cards The cards to add to the players hand.
     */
    virtual void addCards(const std::vector<Card>& cards) = 0;

    /**
     * This function must return the number of cards in the players hand.
     * @return The number of cards in the players hand.
     */
    virtual unsigned getNumCards() const = 0;

    /**
     * This function should return this players name.  It may not contain the
     * '#' character.
     * @return The players name.
     */
    virtual std::string getName() const = 0;

    /** 
     * This function should return a unique ID for this player.  It must begin
     * with a '#'.
     * 
     * @return 
     */
    virtual std::string getID() const = 0;
};

typedef boost::shared_ptr<Player> PlayerPtr;
typedef boost::shared_ptr<const Player> ConstPlayerPtr;
