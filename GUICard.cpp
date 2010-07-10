#include "GUICard.h"
#include "Card.h"
#include <iostream>

GLuint GUICard::cardtex = 0;
int GUICard::CARDX = 70;
int GUICard::CARDY = 96;

static const int SOURCEX = 79;
static const int SOURCEY = 123;

void GUICard::draw(const Card& card)
{
    int col = card.getNum();
    col = (col == Card::ACE) ? 0 : col-1;
    int row;
    switch (card.getSuit())
    {
    case Card::clubs:    row = 0; break;
    case Card::diamonds: row = 1; break;
    case Card::hearts:   row = 2; break;
    case Card::spades:   row = 3; break;
    case Card::none:     row = 4; break;
    }

    glPushMatrix();
    glScalef(CARDX, CARDY, 0);
    glBindTexture(GL_TEXTURE_RECTANGLE, cardtex);
    glBegin(GL_QUADS);
        glTexCoord2i(col * SOURCEX, row * SOURCEY);
        glVertex3f(-0.5, -0.5, 0);

        glTexCoord2i(col * SOURCEX, (row+1) * SOURCEY);
        glVertex3f(-0.5, 0.5, 0);

        glTexCoord2i((col+1) * SOURCEX, (row+1) * SOURCEY);
        glVertex3f(0.5, 0.5, 0);

        glTexCoord2i((col+1) * SOURCEX, row * SOURCEY);
        glVertex3f(0.5, -0.5, 0);
    glEnd();
    glPopMatrix();
}

void GUICard::drawCardBack()
{
    int row = 4;
    int col = 2;

    glPushMatrix();
    glScalef(CARDX, CARDY, 0);
    glBindTexture(GL_TEXTURE_RECTANGLE, cardtex);
    glBegin(GL_QUADS);
        glTexCoord2i(col * SOURCEX, row * SOURCEY);
        glVertex3f(-0.5, -0.5, 0);

        glTexCoord2i(col * SOURCEX, (row+1) * SOURCEY);
        glVertex3f(-0.5, 0.5, 0);

        glTexCoord2i((col+1) * SOURCEX, (row+1) * SOURCEY);
        glVertex3f(0.5, 0.5, 0);

        glTexCoord2i((col+1) * SOURCEX, row * SOURCEY);
        glVertex3f(0.5, -0.5, 0);
    glEnd();
    glPopMatrix();
}
