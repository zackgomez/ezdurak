#pragma once
#include <GL/gl.h>

class Card;

class GUICard
{
public:
    static GLuint cardtex;
    const static int CARDX = 70;
    const static int CARDY = 96;

    static void draw(const Card& card);
    static void drawCardBack();
};

