#include "GUIImpl.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "core/Player.h"
#include "GUIString.h"
#include "GUICard.h"
#include "GUIPlayerView.h"
#include "GUIHumanView.h"

using namespace std;

const int SCREENX = 800;
const int SCREENY = 600;

GUIImpl::GUIImpl() :
    deckString_(),
    discardString_(),
    biscuitStr_()
{
    pthread_mutex_init(&playedCardsLock_, NULL);
    pthread_mutex_init(&playersLock_, NULL);
    validPlayerDisplays_ = false;
    validSizes_ = false;
    validStatus_ = true;
    validBiscuit_ = true;
    discardSize_ = 0;
    deckSize_ = 0;
    humanView_ = NULL;
}

GUIImpl::~GUIImpl()
{
    pthread_mutex_destroy(&playedCardsLock_);
    pthread_mutex_destroy(&playersLock_);
    SDL_Quit();
}

void GUIImpl::run()
{
    initGL();

    if (TTF_Init())
    {
        cout << "TTF_Init: " << TTF_GetError() << '\n';
        exit(2);
    }
    GUIString::font_ = TTF_OpenFont("resources/FreeMonoBold.ttf", 16);
    if (!GUIString::font_)
        cout << "Unable to open font: " << TTF_GetError() << '\n';

    cont_ = true;

    while (cont_)
    {
	render();

	processEvents();

	SDL_Delay(10);
    }

    TTF_Quit();
    SDL_Quit();
}

void GUIImpl::setPlayers(const vector<Player*>& players)
{
    // Lock
    pthread_mutex_lock(&playersLock_);
    // Update
    players_ = players;
    validPlayerDisplays_ = false;

    vector<Player*>::iterator it;
    for (it = players_.begin(); it != players_.end(); it++)
    {
        if ((*it)->getName() == "guiplayer")
        {
            std::rotate(players_.begin(), it, players_.end());
            break;
        }
    }

    // TODO Fix this
    // Make the cards bigger
    GUICard::CARDX *= 1.3;
    GUICard::CARDY *= 1.3;
    // Unlock
    pthread_mutex_unlock(&playersLock_);
}

void GUIImpl::setAttacker(const Player *player)
{
    // Lock
    pthread_mutex_lock(&playersLock_);
    // Update
    attacker_ = player;
    validStatus_ = false;
    // Unlock
    pthread_mutex_unlock(&playersLock_);
}

void GUIImpl::setDefender(const Player *player)
{
    // Lock
    pthread_mutex_lock(&playersLock_);
    // Update
    defender_ = player;
    validStatus_ = false;
    // Unlock
    pthread_mutex_unlock(&playersLock_);
}

void GUIImpl::setTrumpCard(const Card &c)
{
    // Lock
    pthread_mutex_lock(&playedCardsLock_);
    // Update
    trumpCard_ = c;
    // Unlock
    pthread_mutex_unlock(&playedCardsLock_);
}

void GUIImpl::clearPlayedCards()
{
    // Lock
    pthread_mutex_lock(&playedCardsLock_);
    // Update
    attackingCards_.clear();
    defendingCards_.clear();
    // Unlock
    pthread_mutex_unlock(&playedCardsLock_);
}

void GUIImpl::addAttackingCard(const Card& c)
{
    // Lock
    pthread_mutex_lock(&playedCardsLock_);
    // Update
    attackingCards_.push_back(c);
    // Unlock
    pthread_mutex_unlock(&playedCardsLock_);
}

void GUIImpl::addDefendingCard(const Card& c)
{
    // Lock
    pthread_mutex_lock(&playedCardsLock_);
    // Update
    defendingCards_.push_back(c);
    // Unlock
    pthread_mutex_unlock(&playedCardsLock_);
}

void GUIImpl::setPileSizes(int deckSize, int discardSize)
{
    deckSize_ = deckSize;
    discardSize_ = discardSize;
}

void GUIImpl::setBiscuit(const Player *p)
{
    // Lock
    pthread_mutex_lock(&playersLock_);
    // Update
    stringstream ss;
    if (p)
        ss << p->getName() << " is the biscuit!";
    else
        ss << "The game is a tie.";
    biscuit_ = ss.str();
    validBiscuit_ = false;
    // Unlock
    pthread_mutex_unlock(&playersLock_);
}

void GUIImpl::wait(int ms)
{
    SDL_Delay(ms);
}

void GUIImpl::initGL()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_SetVideoMode(SCREENX, SCREENY, 32, SDL_OPENGL);

    glEnable(GL_TEXTURE_RECTANGLE);
    GUICard::cardtex = loadTexture("resources/cards.png");

    glViewport(0, 0, SCREENX, SCREENY);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, SCREENX, SCREENY, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0, 0.5, 0, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GUIImpl::processEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch(e.type)
        {
        case SDL_QUIT:
            cont_ = false; break;
        case SDL_KEYDOWN:
            if (e.key.keysym.sym == SDLK_ESCAPE)
                cont_ = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (e.button.button != 1)
                break;
            int x = e.button.x;
            int y = e.button.y;
            x -= SCREENX/2;
            y -= SCREENY/2 + SCREENY/2 - GUICard::CARDY/2 - 5;
            if (humanView_)
                humanView_->mouseClick(x, y);
            break;
        }
    }
}

void GUIImpl::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Lock
    pthread_mutex_lock(&playedCardsLock_);
    drawPlayedCards();
    drawPiles();
    // Unlock
    pthread_mutex_unlock(&playedCardsLock_);

    drawPlayers();

    SDL_GL_SwapBuffers();
}

void GUIImpl::drawPlayedCards()
{
    glTranslatef(SCREENX/2, SCREENY/2, 0);
    glTranslatef(-1.*GUICard::CARDX - 0.4*GUICard::CARDX,
                 -.6*GUICard::CARDY, 0);
    for (int i = 0; i < attackingCards_.size(); i++)
    {
        // Draw attacking Card
        GUICard::draw(attackingCards_[i]);
        // Move over for defending card
        glTranslatef(GUICard::CARDX * 0.2, 0, 0);
        // Draw defending card if it exists
        if (defendingCards_.size() > i)
            GUICard::draw(defendingCards_[i]);

        // Move over for next set
        if (i == 2)
            glTranslatef(-2*(GUICard::CARDX * 1.2) - 3*GUICard::CARDX*0.2,
                         1.2 * GUICard::CARDY, 0);
        else
            glTranslatef(GUICard::CARDX * 1.2, 0, 0);
    }
}

void GUIImpl::drawPiles()
{
    if (!validSizes_)
    {
        stringstream deckss, discardss;
        deckss << deckSize_;
        discardss << discardSize_;

        deckString_ = GUIStringPtr(new GUIString(deckss.str()));
        discardString_ = GUIStringPtr(new GUIString(discardss.str()));
    }
        
    // Draw the deck and discard pile
    glLoadIdentity();
    glTranslatef(10 + GUICard::CARDX/2, 10 + GUICard::CARDY/2, 0);
    glColor3f(1, 1, 1);
    // First draw the trump, rotated and moved
    if (deckSize_ > 0)
    {
        glPushMatrix();
        glTranslatef(GUICard::CARDY/2 - GUICard::CARDX/2, 0, 0);
        glRotatef(90, 0, 0, 1);
        GUICard::draw(trumpCard_);
        glPopMatrix();
    }
    if (deckSize_ > 1)
    {
        GUICard::drawCardBack();
        glColor3f(1, 0, 0);
        deckString_->draw();
    }

    if (discardSize_ > 0)
    {
        glLoadIdentity();
        glTranslatef(SCREENX - 10 - GUICard::CARDX/2, 10 + GUICard::CARDY/2, 0);
        glColor3f(1, 1, 1);
        GUICard::drawCardBack();
        glColor3f(1, 0, 0);
        discardString_->draw();
    }
}

void GUIImpl::drawPlayers()
{
    // Lock
    pthread_mutex_lock(&playersLock_);
    if (biscuitStr_.get())
    {
        glLoadIdentity();
        glTranslatef(SCREENX/2, SCREENY/2, 0);
        glColor3f(1,1,1);
        biscuitStr_->draw();
    }
    updatePlayers();
    float angle = M_PI/2;
    for (int i = 0; i < players_.size(); i++)
    {
        float x = cos(angle);
        float y = sin(angle);
        const float rx = SCREENX/2 - GUICard::CARDY/2 - 5;
        const float ry = SCREENY/2 - GUICard::CARDY/2 - 5;

        // Center
        glLoadIdentity();
        glTranslatef(SCREENX/2, SCREENY/2, 0);
        // Move outwards
        glTranslatef(x*rx, y*ry, 0);
        // Rotate
        glRotatef(angle*180/M_PI - 90, 0, 0, 1);

        // Draw the player
        playersDisplay_[i]->draw();

        angle += 2*M_PI/ players_.size();
    }
    // Unlock
    pthread_mutex_unlock(&playersLock_);
}

void GUIImpl::updatePlayers()
{
    // Update the name textures, first delete the old ones
    if (!validPlayerDisplays_)
    {
        for (int i = 0; i < playersDisplay_.size(); i++)
            delete playersDisplay_[i];
        playersDisplay_.resize(players_.size());

        humanView_ = new GUIHumanView((GUIPlayer *) players_[0]);
        playersDisplay_[0] = humanView_;
        for (int i = 1; i < players_.size(); i++)
            playersDisplay_[i] = new GUIPlayerView(players_[i]);
        validPlayerDisplays_ = true;
    }
    if (!validStatus_)
    {
        for (int i = 0; i < players_.size(); i++)
        {
            if (players_[i] == attacker_)
                playersDisplay_[i]->setStatus(GUIPlayerView::ATTACKER);
            else if (players_[i] == defender_)
                playersDisplay_[i]->setStatus(GUIPlayerView::DEFENDER);
            else
                playersDisplay_[i]->setStatus(GUIPlayerView::NONE);
        }
        validStatus_ = true;
    }
    if (!validBiscuit_)
    {
        biscuitStr_ = GUIStringPtr(new GUIString(biscuit_));
        validBiscuit_ = true;
    }
}

GLuint GUIImpl::loadTexture(const string& filename)
{
    SDL_Surface *tex;
    tex = IMG_Load(filename.c_str());

    if (!tex)
    {
        cout << "Unable to load image" << IMG_GetError() << '\n';
        exit(1);
    }

    GLenum texture_format;
    if ( tex->format->BytesPerPixel == 4 )
        texture_format = (tex->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
    else if (tex->format->BytesPerPixel == 3 )
        texture_format = (tex->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
    else
    {
        cout << "BPP: " << tex->format->BytesPerPixel << '\n';
        assert(false && "Image is not in the proper format.");
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_RECTANGLE, texture);

    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 4, tex->w, tex->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, tex->pixels);

    SDL_FreeSurface(tex);

    return texture;
}
