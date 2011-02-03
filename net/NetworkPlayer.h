#pragma once
#include "NetworkListener.h"
#include "core/Player.h"
#include <set>
#include "util/Logger.h"

/** 
 * A proxy Player class that forwards all the information necessary for a 
 * player at the other end of the network to play as though there were 
 * this player.  Uses the NetworkListener interface and should be connected
 * to a NetworkGame constructed with a local player.
 *
 * After constructoion, the NetworkListener::getConnection method should be 
 * used to create a valid connection between the NetworkPlayer and remote
 * NetworkGame objects.
 */
class NetworkPlayer :
    public NetworkListener,
    public Player
{
public:
    /** 
     * Creates an unconnected NetworkPlayer object.
     */
    NetworkPlayer();
    /// Destructor
    virtual ~NetworkPlayer();

    // Methods inherited from the Player interface
    virtual void gameStarting(GameAgent *agent);
    virtual Card defend(const Card &attC, Card::cardsuit trump);
    virtual Card attack(std::set<int> playableRanks);
    virtual Card pileOn(std::set<int> playableRanks);
    virtual void addCards(const std::vector<Card>& cards);
    virtual int  getNumCards() const;
    virtual std::string getName() const;
    virtual std::string getID() const;

protected:
    // Overriden from NetworkListener
    virtual bool doHandshake();


private:
    std::set<Card> hand_;
    std::string name_;
    std::string ID_;

    LoggerPtr logger_;
};

typedef boost::shared_ptr<NetworkPlayer> NetworkPlayerPtr;
