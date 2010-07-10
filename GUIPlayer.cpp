#include "GUIPlayer.h"

GUIPlayer::GUIPlayer(const std::string &name) :
    CLIPlayer(name)
{ /* Empty */ }

GUIPlayer::~GUIPlayer()
{ /* Empty */ }

const std::vector<Card>& GUIPlayer::getHand() const
{
    return hand_;
}
