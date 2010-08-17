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
