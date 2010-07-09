#include "GUIImpl.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <cassert>
#include <cmath>
#include <iostream>
#include "GUIString.h"
#include "GUICard.h"
#include "GUIPlayer.h"

using namespace std;

GUIImpl::GUIImpl()
{
    playedCardsLock_ = PTHREAD_MUTEX_INITIALIZER;
    playersLock_     = PTHREAD_MUTEX_INITIALIZER;
    badPlayers_ = true;
}

GUIImpl::~GUIImpl()
{
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
    GUIString::font_ = TTF_OpenFont("resources/FreeMono.ttf", 16);
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
    badPlayers_ = true;
    // Unlock
    pthread_mutex_unlock(&playersLock_);
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

void GUIImpl::wait(int ms)
{
    SDL_Delay(ms);
}

void GUIImpl::initGL()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);

    glEnable(GL_TEXTURE_RECTANGLE);
    GUICard::cardtex = loadTexture("resources/cards3.png");

    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0, 0.5, 0, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GUIImpl::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(400, 300, 0);
    glTranslatef(-1.*GUICard::CARDX - 0.4*GUICard::CARDX,
                 -.6*GUICard::CARDY, 0);
    // Lock
    pthread_mutex_lock(&playedCardsLock_);
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
    // Unlock
    pthread_mutex_unlock(&playedCardsLock_);


    // Lock
    pthread_mutex_lock(&playersLock_);
    // Update the name textures, first delete the old ones
    if (badPlayers_)
    {
        for (int i = 0; i < playersDisplay_.size(); i++)
            delete playersDisplay_[i];
        playersDisplay_.resize(players_.size());

        for (int i = 0; i < players_.size(); i++)
            playersDisplay_[i] = new GUIPlayer(players_[i]);
        badPlayers_ = false;
    }
    // Update
    float angle = M_PI/2;
    for (int i = 0; i < players_.size(); i++)
    {
        float x = cos(angle);
        float y = sin(angle);

        // Center
        glLoadIdentity();
        glTranslatef(400, 300, 0);
        // Move outwards
        glTranslatef(x*345, y*240, 0);
        // Rotate
        glRotatef(angle*180/M_PI - 90, 0, 0, 1);

        // Draw the player
        playersDisplay_[i]->draw();

        angle += 2*M_PI/ players_.size();
    }
    // Unlock
    pthread_mutex_unlock(&playersLock_);

    SDL_GL_SwapBuffers();
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
            cont_ = false; break;
        }
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
