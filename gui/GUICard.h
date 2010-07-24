#pragma once
#include "gl.h"
#include <boost/shared_ptr.hpp>

class Card;

class GUICard;
typedef boost::shared_ptr<GUICard> GUICardPtr;

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
     * Creates a new GUICard for the passed card.  If the passed card is a
     * Card() then the back of a card is drawn when GUICard::draw is called.
     */
    static GUICardPtr create(const Card &c);

    /**
     * Draws a card back at the current location (centered).
     */
    static void drawCardBack();

    /**
     * Draws the card at the current location (centered).  The
     * drawn card will be of size CARDX x CARDY.
     */
    void draw();

private:
    // Private constructors for create idiom
    GUICard(const Card& c);
    GUICard();
    GUICard(const GUICard&);
    void operator=(const GUICard&);

    // Data members
    /// The X,Y coordinates of the card in the cardtex
    int row, col;
};
