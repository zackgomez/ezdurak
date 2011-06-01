#pragma once
#include "GameAgent.h"
#include <vector>
#include <set>
#include "Deck.h"
#include "Player.h"

class Game;
typedef std::auto_ptr<Game> GamePtr;

/**
 * This class encapsulated all the rules and data necessary for a Durak game.
 * It registers listeners and contains players.  The run method will run a
 * game to completion.
 */
class Game : public GameAgent
{
public:
    // Constructors
    Game();
    virtual ~Game();

    /** 
     * Adds a player to the game.  Should be called before run.
     * 
     * @param player The player to add.
     */
    virtual void addPlayer(PlayerPtr player);

    /** 
     * Gets the number of players this game currently has.  A game must have
     * between 2 and 6 players to start.
     * 
     * @return The number of players
     */
    virtual int getNumPlayers() const;

    /**
     * Run the Durak game.
     */
    virtual void run();

    // Constants
    /// The number of cards in a starting hand
    const static unsigned HAND_SIZE;

    // Functions inherited from GameAgent interface
    void addListener(GameListener* listener);
    void removeListener(GameListener* listener);
    Card getTrumpCard() const;
    unsigned getAttacksLeft() const;
    unsigned getDeckSize() const;
    unsigned getDiscardSize() const;
    ConstPlayerPtr getAttacker() const;
    ConstPlayerPtr getDefender() const;
    const std::vector<PlayerPtr> getPlayers() const;
    const std::vector<Card>& getAttackingCards() const;
    const std::vector<Card>& getDefendingCards() const;

protected:
    void setPlayers();

    Deck deck_;
    Card trumpCard_;
    std::vector<PlayerPtr> players_;

    PlayerPtr attacker_;
    PlayerPtr defender_;
    unsigned attackerIdx_;
    unsigned defenderIdx_;
    bool deflectable_; //< True when a defender can deflect
    std::vector<Card> attackingCards_;
    std::vector<Card> defendingCards_;
    std::set<int> playableRanks_;
    std::vector<PlayerPtr> refillOrder_;

    std::set<GameListener*>::iterator lit_;
    std::set<GameListener*> listeners_;

private:

    /** Deals a the initial hands to the players */
    void deal();
    /**
     * Returns true if a hand is a valid starting hand.  I.e. it is not a 
     * misdeal.
     */
    bool validateHands(const std::vector<std::vector<Card> >& hands) const;
    /**
     * Conducts one "round" vs a single defender.
     * @return True if the rounded ended due to a successful defend, false if 
     * the defender lost.
     */
    bool doRound();
    /**
     * Gets an attacking card or the null card if the attackers all pass.
     * @param pileOn if true then the Player::pileOn method will be used 
     * instead of Player::attack.
     * @return The played card or Card() if all attackers passed.
     */
    Card getAttackingCard(bool pileOn = false);
    /**
     * Gets a card from the current defender with the passed in attacking card.
     * Fills in the necessary state variables and returns true/false signifying
     * a successful defend or not.
     * @param attC The card the defender must beat.
     * @return True if the defender successfully beats the card, False if they
     * give up.
     */
    bool getDefendingCard(const Card& attC);
    /** Moves to the next attacker, making sure to skip the current defender. */
    void nextAttacker();
    /** Adds the player to the refill order at the end if they are not already
     * there.
     */
    void addToRefill(PlayerPtr p);
    /**
     * Continues to put cards on playableCards until the attackers all pass or
     * the maximum number of tricks is reached.  It then gives all of the cards
     * played during the round and pile on to the defender.
     */
    void pileOn();
    /** Refills the players hands up to HAND_SIZE. */
    void refill();
    /**
     * Removes the players that have no cards left in their hand as they have 
     * gone out and are no longer playing.
     */
    void removeFinishedPlayers();
    /** Changes the defender_ variable to point to the next defender. */
    void nextDefender(bool successfulDefend);
};

