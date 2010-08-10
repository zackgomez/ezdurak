#include "GUIHumanView.h"
#include "gl.h"
#include <iostream>
#include "GUIPlayer.h"
#include "GUICard.h"

GUIHumanView::GUIHumanView(const GUIPlayer *player) :
    GUIPlayerView(player),
    player_(player),
    queue_(player_->getQueue()),
    passString_(GUIString::create("PASS")),
    giveUpString_(GUIString::create("GIVE UP"))
{
}

GUIHumanView::~GUIHumanView()
{
}

void GUIHumanView::mouseClick(int x, int y)
{
    // If we're not attacking or defending, get outta here!
    if (status_ == NONE)
        return;
    // Figure out if the clicked on a card...
    int numCards = player_->getNumCards();
    int rx = GUICard::CARDX*(0.2*(numCards-1)/2) + GUICard::CARDX/2;
    int ry = GUICard::CARDY/2;

    if ((x < rx && x > -rx) && (y < ry && y > -ry))
    {
        // Figure out which card they clicked
        int idx = std::min((int)((x+rx) / (0.2 * GUICard::CARDX)), numCards-1);
        //std::cout << "Main cards hit - Index: " << idx << '\n';
        queue_.enqueue(idx);
        return;
    }

    int passxmin = rx + GUICard::CARDX*0.2;
    int passxmax = passxmin + GUICard::CARDX;

    if (x < passxmax && x > passxmin && y < ry && y > -ry)
    {
        //std::cout << "Pass card hit\n";
        queue_.enqueue(-1);
    }
}

void GUIHumanView::drawCards()
{
    // Nothing to draw if there are no cards
    if (cards_.size() == 0)
        return;

    glPushMatrix();
    int numCards = player_->getNumCards();
    glTranslatef(-GUICard::CARDX*(0.2*(numCards-1)/2), 0, 0);
    // Draw each card back
    for (int j = 0; j < numCards; j++)
    {
        glColor3f(1,1,1);
        cards_[j]->draw();
        glTranslatef(0.2*GUICard::CARDX, 0, 0);
    }

    if (status_ != NONE)
    {
        glTranslatef(GUICard::CARDX, 0, 0);
        glColor3f(0,0,0);
        GUICard::drawCardBack();
        glColor3f(1,1,1);
        if (status_ == ATTACKER)
            passString_->draw();
        else
            giveUpString_->draw();
    }

    glPopMatrix();
}

void GUIHumanView::update()
{
    if (!dirty_)
        return;

    cards_ = std::vector<GUICardPtr>(player_->getNumCards());
    std::vector<Card> c = player_->getHand();

    for (int i = 0; i < cards_.size(); i++)
        cards_[i] = GUICard::create(c[i]);

    dirty_ = false;
}
