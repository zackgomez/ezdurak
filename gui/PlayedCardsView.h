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
    
private:
    CardHolderImpl attackingCards_;
    CardHolderImpl defendingCards_;
};

