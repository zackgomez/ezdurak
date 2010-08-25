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
    int idx;
    if (attacking)
        idx = nextAttackingIdx_++;
    else
        idx = nextDefendingIdx_++;

    getCardLocation(attacking, idx, x, y);
}

void PlayedCardsView::clearNextLocation()
{
    nextAttackingIdx_ = 0;
    nextDefendingIdx_ = 0;
}

AnimationPtr PlayedCardsView::getAnimation(const std::vector<Card> &cs, CardHolder *target,
                                           int dur, float x1, float y1)
{
    std::list<AnimationPtr> anims;

    for (int i = 0; i < cs.size(); i++)
        anims.push_back(getAnimation(cs[i], target, dur, x1, y1));

    return ParallelAnimation::create(anims);
}

AnimationPtr PlayedCardsView::getAnimation(CardHolder *target, int dur,
                                           float x1, float x2)
{
    std::list<AnimationPtr> anims;

    // Add the attacking cards
    const std::vector<Card> &cards = attackingCards_.getCards();
    for (int i = 0; i  < cards.size(); i++)
        anims.push_back(getAnimation(cards[i], target, dur, x1, x2));
    // And the defending cards
    const std::vector<Card> &dcards = defendingCards_.getCards();
    for (int i = 0; i  < dcards.size(); i++)
        anims.push_back(getAnimation(dcards[i], target, dur, x1, x2));

    // Create and return the parallel animation
    return ParallelAnimation::create(anims);
}

void PlayedCardsView::getCardLocation(bool attacking, int index, float &x, float &y)
{
    x = 400 - 1.*GUICard::CARDX- 0.4*GUICard::CARDX;
    y = 300 - .6*GUICard::CARDY;

    if (index > 2)
        y += 1.2 * GUICard::CARDY;

    x += (index % 3) * 1.4 * GUICard::CARDX;
    if (!attacking)
        x += 0.2 * GUICard::CARDX;
}


AnimationPtr PlayedCardsView::getAnimation(const Card &c, CardHolder *target,
                                           int dur, float x1, float y1)
{
    CardHolderImpl *source = NULL;
    float x0, y0;
    bool attacking;
    if (attackingCards_.contains(c))
    {
        source = &attackingCards_;
        attacking = true;
    }
    else if (defendingCards_.contains(c))
    {
        source = &defendingCards_;
        attacking = false;
    }
    else
        assert(false && "Card not found");

    std::vector<Card>::iterator it = std::find(source->getCards().begin(),
                                               source->getCards().end(), c);
    assert(it != source->getCards().end());
    int idx = it - source->getCards().begin();

    getCardLocation(attacking, idx, x0, y0);

    return MoveAnimation::create(c, source, target, dur, x0, y0, x1, y1);
                                 
}