#include "GUIPlayer.h"
#include "Player.h"
#include "GUIString.h"
#include "GUICard.h"

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
    int numCards = player_->getNumCards();

    glPushMatrix();
    /*
    glTranslatef(-xvel*GUICard::CARDX*(0.2*(numCards-1)/2),
                 -yvel*GUICard::CARDX*(0.2*(numCards-1)/2), 0);
                 */
    // Draw each card back
    for (int j = 0; j < numCards; j++)
    {
        GUICard::drawCardBack();
        glTranslatef(0.2*GUICard::CARDX, 0, 0);
    }
    glPopMatrix();

    // Draw the name
    glPushMatrix();
    glTranslatef(0, -(GUICard::CARDX/2 + 20), 0);
    glTranslatef(400, 300, 0);
    name_->draw();
    glPopMatrix();
}

void GUIPlayer::setStatus(Status status)
{
    status_ = status;
}
