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

GUIPlayer::~GUIPlayer()
{
    delete name_;
}

void GUIPlayer::draw()
{
}

void GUIPlayer::setStatus(Status status)
{
    status_ = status;
}
