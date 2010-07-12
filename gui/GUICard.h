#pragma once
#include "gl.h"

class Card;

class GUICard
{
public:
    static GLuint cardtex;
    static int CARDX;
    static int CARDY;

    static void draw(const Card& card);
    static void drawCardBack();
};

