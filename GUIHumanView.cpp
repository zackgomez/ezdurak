#include "GUIHumanView.h"
#include <GL/gl.h>
#include <iostream>
#include "GUIPlayer.h"
#include "GUICard.h"

GUIHumanView::GUIHumanView(const GUIPlayer *player) :
    GUIPlayerView(player),
    player_(player),
    passString_("PASS"),
    giveUpString_("GIVE UP")
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
        //std::cout << "Main cards hit\n";
    }

    int passxmin = rx + GUICard::CARDX*0.2;
    int passxmax = passxmin + GUICard::CARDX;

    if (x < passxmax && x > passxmin && y < ry && y > -ry)
    {
        //std::cout << "Pass card hit\n";
    }
}

void GUIHumanView::draw()
{
    drawCards();
    GUIPlayerView::drawName();
}

void GUIHumanView::drawCards()
{
    const std::vector<Card> cards = player_->getHand();
    int numCards = cards.size();

    glPushMatrix();
    glTranslatef(-GUICard::CARDX*(0.2*(numCards-1)/2), 0, 0);
    // Draw each card back
    for (int j = 0; j < numCards; j++)
    {
        glColor3f(1,1,1);
        GUICard::draw(cards[j]);
        glTranslatef(0.2*GUICard::CARDX, 0, 0);
    }

    if (status_ != NONE)
    {
        glTranslatef(GUICard::CARDX, 0, 0);
        glColor3f(0,0,0);
        GUICard::drawCardBack();
        glColor3f(1,1,1);
        if (status_ == ATTACKER)
            passString_.draw();
        else
            giveUpString_.draw();
    }

    glPopMatrix();
}
