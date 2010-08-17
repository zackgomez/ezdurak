#include "GUIPlayerView.h"
#include "core/Player.h"
#include "GUIString.h"
#include "GUICard.h"

GLuint GUIPlayerView::attackEmblem = 0;
GLuint GUIPlayerView::defendEmblem = 0;

GUIPlayerView::GUIPlayerView(const Player *player) :
    player_(player),
    name_(GUIString::create(player->getName())),
    status_(NONE)
{
    // Get the initial hand...
    for (int i = 0; i < player->getNumCards(); i++)
        cards_.addCard(Card());
}

GUIPlayerView::~GUIPlayerView()
{
}

void GUIPlayerView::draw()
{
    drawCards();
    drawName();
}

void GUIPlayerView::setStatus(Status status)
{
    status_ = status;
}

CardHolder * GUIPlayerView::getCardHolder()
{
    return &cards_;
}

void GUIPlayerView::drawName()
{
    glPushMatrix();
    glTranslatef(0, -(GUICard::CARDY/2 + 15), 0);
    if (status_ == ATTACKER)
        glColor3f(1,0,0);
    else if (status_ == DEFENDER)
        glColor3f(0,0,1);
    else
        glColor3i(0,0,0);
    name_->draw();

    glColor3f(1,1,1);
    glTranslatef(name_->getWidth()/2 + 35./2 + 3, -5, 0);
    if (status_ == ATTACKER)
        drawEmblem(attackEmblem);
    else if (status_ == DEFENDER)
        drawEmblem(defendEmblem);

    glPopMatrix();
}

void GUIPlayerView::drawCards()
{
    int numCards = cards_.getNumCards();
    glPushMatrix();
    glTranslatef(-GUICard::CARDX*(0.2*(numCards-1)/2), 0, 0);
    glColor3f(1,1,1);
    // Draw each card
    for (int j = 0; j < numCards; j++)
    {
        GUICard::drawCardBack();
        glTranslatef(0.2*GUICard::CARDX, 0, 0);
    }
    glPopMatrix();
}

void GUIPlayerView::drawEmblem(GLuint tex)
{
    glPushMatrix();
    glBindTexture(GL_TEXTURE_RECTANGLE, tex);
    glScalef(35, 35, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-0.5, -0.5, 0);

        glTexCoord2f(0, 35);
        glVertex3f(-0.5, 0.5, 0);

        glTexCoord2f(35, 35);
        glVertex3f(0.5, 0.5, 0);

        glTexCoord2f(35, 0);
        glVertex3f(0.5, -0.5, 0);
    glEnd();
    glPopMatrix();
}
