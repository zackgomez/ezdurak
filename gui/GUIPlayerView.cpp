#include "GUIPlayerView.h"
#include "core/Player.h"
#include "GUIString.h"
#include "GUICard.h"

GLuint GUIPlayerView::attackEmblem = 0;
GLuint GUIPlayerView::defendEmblem = 0;
GLuint GUIPlayerView::defendLostEmblem = 0;
const int GUIPlayerView::EMBLEM_SIZE = 35;

GUIPlayerView::GUIPlayerView(const Player *player) :
    player_(player),
    name_(GUIString::create(player->getName())),
    status_(NONE)
{
    // Get the initial hand...
    for (unsigned i = 0; i < player->getNumCards(); i++)
        cards_.addCard(Card());
}

GUIPlayerView::~GUIPlayerView()
{
}

void GUIPlayerView::draw(bool animating)
{
    drawCards(animating);
    drawName(animating);
}

void GUIPlayerView::setStatus(Status status)
{
    status_ = status;
}

CardHolder * GUIPlayerView::getCardHolder()
{
    return &cards_;
}

void GUIPlayerView::drawName(bool animating)
{
    glPushMatrix();
    glTranslatef(0, -(GUICard::CARDY/2 + 15), 0);
    if (status_ == ATTACKER)
        glColor3f(1,0,0);
    else if (status_ == DEFENDER || status_ == DEFENDERLOST)
        glColor3f(0,0,1);
    else
        glColor3i(0,0,0);
    name_->draw();

    glColor3f(1,1,1);
    glTranslatef(name_->getWidth()/2 + EMBLEM_SIZE/2. + 3, -5, 0);
    if (status_ == ATTACKER)
        drawEmblem(attackEmblem);
    else if (status_ == DEFENDER)
        drawEmblem(defendEmblem);
    else if (status_ == DEFENDERLOST)
        drawEmblem(defendLostEmblem);

    glPopMatrix();
}

void GUIPlayerView::drawCards(bool animating)
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
    glScalef(EMBLEM_SIZE, EMBLEM_SIZE, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-0.5, -0.5, 0);

        glTexCoord2f(0, EMBLEM_SIZE);
        glVertex3f(-0.5, 0.5, 0);

        glTexCoord2f(EMBLEM_SIZE, EMBLEM_SIZE);
        glVertex3f(0.5, 0.5, 0);

        glTexCoord2f(EMBLEM_SIZE, 0);
        glVertex3f(0.5, -0.5, 0);
    glEnd();
    glPopMatrix();
}
