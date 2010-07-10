#include "GUIHumanView.h"
#include <GL/gl.h>
#include "GUIPlayer.h"
#include "GUICard.h"

GUIHumanView::GUIHumanView(const GUIPlayer *player) :
    GUIPlayerView(player),
    player_(player)
{ /* Empty */ }

GUIHumanView::~GUIHumanView()
{ /* Empty */ }

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
    glPopMatrix();
}
