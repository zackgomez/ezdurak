#include "GUIImpl.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <cassert>
#include <iostream>

using namespace std;

GUIImpl::GUIImpl()
{
    playedCardsLock = PTHREAD_MUTEX_INITIALIZER;
}

GUIImpl::~GUIImpl()
{
    SDL_Quit();
}

void GUIImpl::run()
{
    initGL();
    cont = true;

    while (cont)
    {
	render();

	processEvents();

	SDL_Delay(10);
    }

    SDL_Quit();
}

void GUIImpl::setPlayedCards(const vector<Card>& newCards)
{
    // Lock
    pthread_mutex_lock(&playedCardsLock);
    // Update
    playedCards = newCards;
    // Unlock
    pthread_mutex_unlock(&playedCardsLock);
}

void GUIImpl::initGL()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);

    glEnable(GL_TEXTURE_RECTANGLE);
    cardtex = loadTexture("resources/cards3.png");

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
    glBindTexture(GL_TEXTURE_RECTANGLE, cardtex);

    glScalef(70, 96, 0);
    // Lock
    pthread_mutex_lock(&playedCardsLock);
    for (int i = 0; i < playedCards.size(); i++)
    {
        glColor3f(1,1,1);

        int col = playedCards[i].getNum();
        col = (col == Card::ACE) ? 0 : col-1;
        int row;
        switch (playedCards[i].getSuit())
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
    pthread_mutex_unlock(&playedCardsLock);

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
            cont = false; break;
        case SDL_KEYDOWN:
            cont = false; break;
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
