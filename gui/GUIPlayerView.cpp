#include "GUIPlayerView.h"
#include "core/Player.h"
#include "GUIString.h"
#include "GUICard.h"

GUIPlayerView::GUIPlayerView(const Player *player) :
    player_(player),
    name_(GUIString::create(player->getName())),
    status_(NONE),
    dirty_(true)
{
}

GUIPlayerView::~GUIPlayerView()
{
}

void GUIPlayerView::draw()
{
    update();
    drawCards();
    drawName();
}

void GUIPlayerView::dirty()
{
    dirty_ = true;
}

void GUIPlayerView::setStatus(Status status)
{
    status_ = status;
}

void GUIPlayerView::drawName()
{
    glPushMatrix();
    glTranslatef(0, -(GUICard::CARDY/2 + 15), 0);
    if (status_ == ATTACKER)
        glColor3f(1,0,0);
    else if (status_ == DEFENDER)
        glColor3f(0,0,1);
    else
        glColor3i(0,0,0);
    name_->draw();
    glPopMatrix();
}

void GUIPlayerView::drawCards()
{
    assert(player_->getNumCards() == cards_.size());

    glPushMatrix();
    glTranslatef(-GUICard::CARDX*(0.2*(cards_.size()-1)/2), 0, 0);
    // Draw each card
    for (int j = 0; j < cards_.size(); j++)
    {
        glColor3f(1,1,1);
        cards_[j]->draw();
        glTranslatef(0.2*GUICard::CARDX, 0, 0);
    }
    glPopMatrix();
}

void GUIPlayerView::update()
{
    // If the nothing has changed, no need to update
    if (!dirty_)
        return;

    // Get rid of old cards
    cards_ = std::vector<GUICardPtr>(player_->getNumCards());

    for (int i = 0; i < cards_.size(); i++)
        cards_[i] = GUICard::create(Card());

    // No longer dirty
    dirty_ = false;
}
