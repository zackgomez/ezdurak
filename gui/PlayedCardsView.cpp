#include "PlayedCardsView.h"
#include "GUICard.h"

PlayedCardsView::PlayedCardsView()
{ /* Empty */ }

CardHolder * PlayedCardsView::getAttackingHolder()
{
    return &attackingCards_;
}

CardHolder * PlayedCardsView::getDefendingHolder()
{
    return &defendingCards_;
}

void PlayedCardsView::clear()
{
    attackingCards_.getCards().clear();
    defendingCards_.getCards().clear();
}

void PlayedCardsView::render()
{
    glTranslatef(-1.*GUICard::CARDX - 0.4*GUICard::CARDX,
                 -.6*GUICard::CARDY, 0);
    for (int i = 0; i < attackingCards_.getNumCards(); i++)
    {
        // Draw attacking Card
        GUICard::draw(attackingCards_.getCards()[i]);
        // Move over for defending card
        glTranslatef(GUICard::CARDX * 0.2, 0, 0);
        // Draw defending card if it exists
        if (defendingCards_.getNumCards() > i)
            GUICard::draw(defendingCards_.getCards()[i]);

        // Move over for next set
        if (i == 2)
            glTranslatef(-2*(GUICard::CARDX * 1.2) - 3*GUICard::CARDX*0.2,
                         1.2 * GUICard::CARDY, 0);
        else
            glTranslatef(GUICard::CARDX * 1.2, 0, 0);
    }
}

void PlayedCardsView::getNextCardLocation(bool attacking, float &x, float &y)
{
    x = 400 - 1.*GUICard::CARDX- 0.4*GUICard::CARDX;
    y = 300 - .6*GUICard::CARDY;
    int idx;
    if (attacking)
        idx = nextAttackingIdx_++;
    else
        idx = nextDefendingIdx_++;

    if (idx > 2)
        y += 1.2 * GUICard::CARDY;

    x += (idx % 3) * 1.4 * GUICard::CARDX;
    if (!attacking)
        x += 0.2 * GUICard::CARDX;
}

void PlayedCardsView::clearNextLocation()
{
    nextAttackingIdx_ = 0;
    nextDefendingIdx_ = 0;
}
