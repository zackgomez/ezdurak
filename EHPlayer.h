/* EHPlayer.h */

#pragma once
#include "AIPlayer.h"

class EHPlayer :
    public AIPlayer
{
public:
    EHPlayer(GameAgent* agent, const std::string& name,
             const std::vector<Card>& hand);
    virtual ~EHPlayer();

    // Override attack
    virtual Card attack(std::set<int> playableRanks = std::set<int>());
    

private:
};



