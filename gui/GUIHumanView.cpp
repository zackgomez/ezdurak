#include "GUIHumanView.h"
#include "gl.h"
#include <iostream>
#include "GUIPlayer.h"
#include "GUICard.h"

GUIHumanView::GUIHumanView(const GUIPlayer *player) :
    GUIPlayerView(player),
    cards_(player, player->getTrumpCard()),
    player_(player),
    queue_(player_->getQueue()),
    passString_(GUIString::create("PASS")),
    giveUpString_(GUIString::create("GIVE UP")),
    dontDeflectString_(GUIString::create("DONT DEFLECT"))
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
    // This should only happen when player and cards are in sync
    assert(player_->getNumCards() == cards_.getNumCards());
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

CardHolder * GUIHumanView::getCardHolder()
{
    return &cards_;
}

void GUIHumanView::drawCards(bool animating)
{
    // Nothing to draw if there are no cards
    if (cards_.getNumCards() == 0)
        return;

    glPushMatrix();
    int numCards = cards_.getNumCards();
    glTranslatef(-GUICard::CARDX*(0.2*(numCards-1)/2), 0, 0);
    // Draw each card back
    for (int j = 0; j < numCards; j++)
    {
        glColor3f(1,1,1);
        GUICard::draw(cards_.getCards()[j]);
        glTranslatef(0.2*GUICard::CARDX, 0, 0);
    }

    GUIPlayer::Action action = player_->getAction();
    if (action != GUIPlayer::NONE && !animating)
    {
        glTranslatef(GUICard::CARDX, 0, 0);
        glColor3f(0,0,0);
        GUICard::drawCardBack();
        glColor3f(1,1,1);
        if (action == GUIPlayer::ATTACK)
            passString_->draw();
        else if (action == GUIPlayer::DEFEND)
            giveUpString_->draw();
        else if (action == GUIPlayer::DEFLECT)
            dontDeflectString_->draw();
        else
            assert(false && "Unknown action");
    }

    glPopMatrix();
}
