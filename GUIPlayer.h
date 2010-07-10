#pragma once
#include "CLIPlayer.h"

class GUIPlayer :
    public CLIPlayer
{
public:
    GUIPlayer(const std::string &name);
    ~GUIPlayer();

    const std::vector<Card>& getHand() const;
private:
};

