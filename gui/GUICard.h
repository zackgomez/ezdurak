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
     * Draws a card back at the current location (centered).
     */
    static void drawCardBack();

    /**
     * Draws the card at the current location (centered).  The
     * drawn card will be of size CARDX x CARDY.
     */
    static void draw(Card c);

private:
    // Private constructors
    GUICard();
    GUICard(const GUICard&);
    void operator=(const GUICard&);

    // Data members
    /// The X,Y coordinates of the card in the cardtex
    int row, col;
};
