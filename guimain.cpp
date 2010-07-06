#include "GUIListener.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <ctime>
#include "Deck.h"
#include "Card.h"

using namespace std;

void initGL();
void render();

GLuint loadTexture(string filename);
void drawCard(int row, int col);

GLuint cardtex = 0;
vector<Card> playedCards;

int main(int argc, char **argv)
{
    srand(time(NULL));

    Deck deck;
    deck.shuffle();
    playedCards = deck.deal(6);

    initGL();
    SDL_Event e;
    bool cont = true;
    while (cont)
    {
        render();

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

        SDL_Delay(10);
    }

    SDL_Quit();
    return 0;
}

void initGL()
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
};

void render()
{

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_RECTANGLE);
    glBindTexture(GL_TEXTURE_RECTANGLE, cardtex);

    glScalef(70, 94, 0);
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

        cout << "Card: " << playedCards[i] << " row: " << row << " col: " << col << '\n';

        drawCard(row, col);

        glTranslatef(1.2, 0, 0);
    }
    cout << "----------------\n";

    SDL_GL_SwapBuffers();
}


GLuint loadTexture(string filename)
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

void drawCard(int row, int col)
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
