#pragma once
#include "CardHolderImpl.h"
#include "Animation.h"

class PlayedCardsView
{
public:
    PlayedCardsView();

    CardHolder* getAttackingHolder();
    CardHolder* getDefendingHolder();

    void clear();
    void render();

    void getNextCardLocation(bool attacking, float &x, float &y);
    void clearNextLocation();

    AnimationPtr getAnimation(const std::vector<Card> &cs, CardHolder *target,
                              int dur, float x1, float y1);
    AnimationPtr getAnimation(CardHolder *target, int dur, float x1, float y1);
    
private:
    // Helper functions
    void getCardLocation(bool attacking, int index, float &x, float &y);
    AnimationPtr getAnimation(const Card &c, CardHolder *target,
                              int dur, float x1, float y1);

    // Data members
    CardHolderImpl attackingCards_;
    CardHolderImpl defendingCards_;

    int nextAttackingIdx_;
    int nextDefendingIdx_;
};

