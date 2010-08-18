#pragma once
#include "CardHolderImpl.h"

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
    
private:
    CardHolderImpl attackingCards_;
    CardHolderImpl defendingCards_;

    int nextAttackingIdx_;
    int nextDefendingIdx_;
};

