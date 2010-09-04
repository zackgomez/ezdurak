#pragma once
#include "Player.h"
#include <vector>
#include <string>
#include "Card.h"

/**
 * This class implements the noninteresting methods of Player.  It takes care
 * of adding cards, returning the number of cards and returning the name.  It
 * also has a hand_, name_ and agent_ member available for use by subclasses.
 * The attack, defend and pileOn methods are still pure virtual.
 */
class PlayerImpl :
    public Player
{
public:
    /** 
     * Creates a PlayerImpl.
     * 
     * @param name The player's name.
     * @param ID
     */
    PlayerImpl(const std::string &name, const std::string &ID = "");
    virtual ~PlayerImpl();

    /** 
     * Creates a unique ID suitable for use as the ID parameter.
     * 
     * @return A unique ID.
     */
    static std::string getUniqueID();

    /**
     * Called at the start of the game.
     */
    virtual void gameStarting(GameAgent *agent);

    virtual Card defend(const Card& attackingCard, Card::cardsuit trump) = 0;
    virtual Card attack(std::set<int> playableRanks) = 0;
    virtual Card pileOn(std::set<int> playableRanks) = 0;

    virtual void addCards(const std::vector<Card>& cards);
    virtual int getNumCards() const;
    virtual std::string getName() const;
    virtual std::string getID() const;

    void print() const;

protected:
    /// The Player's hand
    std::vector<Card> hand_;
    /// The Player's name
    std::string name_;
    /// THe Player's ID
    std::string ID_;
    /// The GameAgent for the game, only valid after gameStarting.
    GameAgent *agent_;
};

