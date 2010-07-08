#include "GUIImpl.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <cassert>
#include <cmath>
#include <iostream>
#include "Player.h"

using namespace std;

const float CARDX = 70;
const float CARDY = 96;

GUIImpl::GUIImpl()
{
    playedCardsLock_ = PTHREAD_MUTEX_INITIALIZER;
    playersLock_     = PTHREAD_MUTEX_INITIALIZER;
    font = 0;
    badNames_ = true;
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
    font = TTF_OpenFont("resources/FreeMono.ttf", 18);
    if (!font)
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
    badNames_ = true;
    // Unlock
    pthread_mutex_unlock(&playersLock_);
}


void GUIImpl::setPlayedCards(const vector<Card>& newCards)
{
    // Lock
    pthread_mutex_lock(&playedCardsLock_);
    // Update
    playedCards_ = newCards;
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
    cardtex_ = loadTexture("resources/cards3.png");

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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_RECTANGLE);
    glBindTexture(GL_TEXTURE_RECTANGLE, cardtex_);

    glTranslatef(113, 300-CARDY/2, 0);
    glScalef(CARDX, CARDY, 0);
    // Lock
    pthread_mutex_lock(&playedCardsLock_);
    for (int i = 0; i < playedCards_.size(); i++)
    {
        glColor3f(1,1,1);

        int col = playedCards_[i].getNum();
        col = (col == Card::ACE) ? 0 : col-1;
        int row;
        switch (playedCards_[i].getSuit())
        {
        case Card::clubs:    row = 0; break;
        case Card::diamonds: row = 1; break;
        case Card::hearts:   row = 2; break;
        case Card::spades:   row = 3; break;
        }

        drawCard(row, col);

        if (i % 2 == 0)
            glTranslatef(.2, 0, 0);
        else
            glTranslatef(1.2, 0, 0);
    }
    // Unlock
    pthread_mutex_unlock(&playedCardsLock_);


    // Lock
    pthread_mutex_lock(&playersLock_);
    // Update the name textures, first delete the old ones
    if (badNames_)
    {
        for (int i = 0; i < nameTextures_.size(); i++)
            glDeleteTextures(1, &nameTextures_[i]);
        nameTextures_.resize(players_.size());
        nameTextureSizes_.resize(players_.size());

        for (int i = 0; i < players_.size(); i++)
            makeStringTexture(i, players_[i]->getName());
        badNames_ = false;
    }
    // Update
    float angle = M_PI/2;
    for (int i = 0; i < players_.size(); i++)
    {
        int numCards = players_[i]->getNumCards();
        float x = cos(angle);
        float y = sin(angle);
        float xvel = y;
        float yvel = -x;

        // Center
        glLoadIdentity();
        glTranslatef(400-CARDX/2, 300-CARDY/2, 0);
        // Move outwards
        glTranslatef(x*350, y*235, 0);
        // Adjust so cards are centered
        glTranslatef(-xvel*CARDX*(0.2*(numCards-1)/2),
                     -yvel*CARDX*(0.2*(numCards-1)/2), 0);
        // Draw each card back
        for (int j = 0; j < numCards; j++)
        {
            glPushMatrix();
            glBindTexture(GL_TEXTURE_RECTANGLE, cardtex_);
            glScalef(CARDX, CARDY, 0);
            drawCard(4, 2);
            glPopMatrix();
            glTranslatef(xvel*0.2*CARDX, yvel*0.2*CARDX, 0);
        }

        // Draw the name
        glLoadIdentity();
        glTranslatef(400, 300, 0);
        glTranslatef(x*300, y*170, 0);
        glRotatef(180*angle/M_PI - 90, 0, 0, 1);
        glBindTexture(GL_TEXTURE_RECTANGLE, nameTextures_[i]);
        drawName(i);

        angle += M_PI/2.;
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

void GUIImpl::drawCard(int row, int col)
{
    glBegin(GL_QUADS);
        glTexCoord2i(col * 79, row * 123);
        glVertex3f(0, 0, 0);

        glTexCoord2i(col * 79, (row+1) * 123);
        glVertex3f(0, 1, 0);

        glTexCoord2i((col+1) * 79, (row+1) * 123);
        glVertex3f(1, 1, 0);

        glTexCoord2i((col+1) * 79, row * 123);
        glVertex3f(1, 0, 0);
    glEnd();
}

void GUIImpl::drawName(int i)
{
    int x = nameTextureSizes_[i].first;
    int y = nameTextureSizes_[i].second;
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);
        glVertex3f(-x/2, -y/2, 0);

        glTexCoord2i(0, y);
        glVertex3f(-x/2, y/2, 0);

        glTexCoord2i(x, y);
        glVertex3f(x/2, y/2, 0);

        glTexCoord2i(x, 0);
        glVertex3f(x/2, -y/2, 0);
    glEnd();
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

void GUIImpl::makeStringTexture(int i, const string& str)
{
    SDL_Color color = {0,0,0};
    SDL_Surface *fontsurf = TTF_RenderText_Blended(font, str.c_str(), color);

    if (!fontsurf)
    {
        cout << "Tried to render.. " << str << '\n';
        cout << "Unable to render text: " << TTF_GetError() << '\n';
        return;
    }

    GLenum texture_format;
    if (fontsurf->format->BytesPerPixel == 4 )
        texture_format = (fontsurf->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
    else if (fontsurf->format->BytesPerPixel == 3 )
        texture_format = (fontsurf->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
    else
    {
        cout << "BPP: " << fontsurf->format->BytesPerPixel << '\n';
        assert(false && "Image is not in the proper format.");
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_RECTANGLE, texture);

    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, 4, fontsurf->w, fontsurf->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, fontsurf->pixels);
    nameTextureSizes_[i].first = fontsurf->w;
    nameTextureSizes_[i].second = fontsurf->h;
    SDL_FreeSurface(fontsurf);

    nameTextures_[i] = texture;
}
