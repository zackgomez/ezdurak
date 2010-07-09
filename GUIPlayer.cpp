#include "GUIPlayer.h"
#include "Player.h"
#include "GUIString.h"

GUIPlayer::GUIPlayer(const Player *player) :
    player_(player),
    name_(0),
    status_(NONE)
{
    name_ = new GUIString(player->getName());
}
