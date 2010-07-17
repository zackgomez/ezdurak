#pragma once
#include "gl.h"

class Card;

/**
 * Static class that has methods and variables useful for rendering cards
 * on the screen.
 */
class GUICard
{
public:
    /// The texture containing all of the cards and the card back image
    static GLuint cardtex;
    /// Size in the x dimension of a default drawn card
    static int CARDX;
    /// Size in the y dimension of a default drawn card
    static int CARDY;

    /**
     * Draws the specified card at the current location (centered).  The
     * drawn card will be of size CARDX x CARDY.
     */
    static void draw(const Card& card);

    /**
     * Draws a card back at the current location (centered).  The drawn card
     * back will be of size CARDX x CARDY.
     */
    static void drawCardBack();
};

