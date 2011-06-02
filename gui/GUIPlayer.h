#pragma once
#include "util/SynchronizedQueue.h"
#include "core/PlayerImpl.h"

/** 
 * This is an implemenation of the Player interface for use in the GUI.  Has
 * additional methods for communicating information to the GUI that normal 
 * players don't need.
 */
class GUIPlayer :
    public PlayerImpl
{
public:
    enum Action
    {
        NONE,
        ATTACK,
        DEFEND,
        DEFLECT,
    };

    GUIPlayer(const std::string &name, SynchronizedQueue<int> &q);
    ~GUIPlayer();

    const std::vector<Card>& getHand() const;
    SynchronizedQueue<int>& getQueue() const;
    const Card getTrumpCard() const;
    /**
     * Returns the Action that this player is currently waiting on.
     */
    Action getAction() const;

    virtual void gameStarting(GameAgent *agent);
    virtual Card defend(const Card& attackingCard, Card::cardsuit trump);
    virtual Card attack(std::set<int> playableRanks = std::set<int>());
    virtual Card pileOn(std::set<int> playableRanks);
    virtual Card deflect(const Card& attackingCard);
    virtual void addCards(const std::vector<Card>& cards);
private:
    void sortHand();
    SynchronizedQueue<int> &queue_;
    Action action_;
};

typedef boost::shared_ptr<GUIPlayer> GUIPlayerPtr;
