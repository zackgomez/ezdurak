#pragma warning(disable : 4018 4244)
#include "InGameState.h"
#include <sstream>
#include <cmath>
#include "core/Game.h"
#include "GUIPlayerView.h"
#include "GUIHumanView.h"
#include "GUIPlayer.h"
#include "GUICard.h"
#include "GUIApp.h"
#include "ai/AIPlayer.h"
#include "GameOverState.h"
#include "QuitState.h"
#include "Animation.h"

#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

using namespace std;

void* game_thread_main(void *gameobj)
{
    srand(time(NULL));
    Game *game = (Game*) gameobj;
    game->run();

    pthread_exit(NULL);
    return NULL;
}

GUIStatePtr InGameState::create(int numPlayers)
{
    GUIStatePtr ret(new InGameState(numPlayers));
    return ret;
}


InGameState::InGameState(int numPlayers) :
    trumpCard_(GUICard::create(Card())),
    deckSize_(0),
    discardSize_(0),
    gameOver_(false),
    validPlayerDisplays_(false),
    humanView_(NULL),
    validStatus_(true),
    validSizes_(false)
{
    pthread_mutex_init(&guiLock_, NULL);

    assert(numPlayers >= 2 && numPlayers <= 6);
    std::vector<PlayerPtr> players(numPlayers);
    players[0] = PlayerPtr(new GUIPlayer("guiplayer", queue_));
    for (int i = 1; i < players.size(); i++)
    {
        std::stringstream ss;
        ss << "AIPlayer" << i;
        std::string name = ss.str();
        players[i] = PlayerPtr(new AIPlayer(name));
    }
    std::random_shuffle(players.begin(), players.end());

    game = new Game(players);
    game->addListener(this);
    agent_ = game;
    pthread_create(&game_thread, NULL, game_thread_main, game);
}

InGameState::~InGameState()
{
    pthread_cancel(game_thread);
    pthread_join(game_thread, NULL);
    delete game;

    for (int i = 0; i < playersDisplay_.size(); i++)
        delete playersDisplay_[i];
    pthread_mutex_destroy(&guiLock_);
}

void InGameState::render()
{
    // Clear
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Lock
    pthread_mutex_lock(&guiLock_);
    drawPlayedCards();
    drawPiles();
    drawPlayers();
    // Unlock
    pthread_mutex_unlock(&guiLock_);

}

bool InGameState::needsTransition() const
{
    return gameOver_ || next_.get() != NULL;
}

GUIStatePtr InGameState::nextState()
{
    if (gameOver_)
        next_ = GameOverState::create(biscuit_);
    return next_;
}

void InGameState::processEvent(SDL_Event &e)
{
    switch(e.type)
    {
    case SDL_QUIT:
        assert(false && "Should not have recieved a QUIT event"); break;
    case SDL_KEYDOWN:
        if (e.key.keysym.sym == SDLK_ESCAPE)
            next_ = QuitState::create();
        else if (e.key.keysym.sym == SDLK_n)
            next_ = InGameState::create(4);
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (e.button.button != 1)
            break;
        int x = e.button.x;
        int y = e.button.y;
        x -= GUIApp::SCREENX/2;
        y -= GUIApp::SCREENY/2 + GUIApp::SCREENY/2 - GUICard::CARDY/2 - 5;
        if (humanView_)
            humanView_->mouseClick(x, y);
        break;
    }
}

void InGameState::gameStart()
{
    pthread_mutex_lock(&guiLock_);

    const vector<PlayerPtr> players = agent_->getPlayers();
    setPlayers(players);

    trumpCard_ = GUICard::create(agent_->getTrumpCard());

    pthread_mutex_unlock(&guiLock_);
}

void InGameState::gameOver(ConstPlayerPtr biscuit)
{
    pthread_mutex_lock(&guiLock_);

    attacker_ = PlayerPtr();
    defender_ = PlayerPtr();
    biscuit_ = biscuit;
    gameOver_ = true;
    validStatus_ = false;

    pthread_mutex_unlock(&guiLock_);
}

void InGameState::newRound(ConstPlayerPtr attacker, ConstPlayerPtr defender)
{
    pthread_mutex_lock(&guiLock_);
    
    attackingCards_.clear();
    defendingCards_.clear();
    deckSize_ = agent_->getDeckSize();
    discardSize_ = agent_->getDiscardSize();

    attacker_ = attacker;
    defender_ = defender;
    validStatus_ = false;

    pthread_mutex_unlock(&guiLock_);
}

void InGameState::attackerPassed(ConstPlayerPtr newAttacker)
{
    pthread_mutex_lock(&guiLock_);
    attacker_ = newAttacker;
    validStatus_ = false;
    pthread_mutex_unlock(&guiLock_);
}

void InGameState::endRound(bool successfulDefend)
{
    pthread_mutex_lock(&guiLock_);
    wait(400);
    pthread_mutex_unlock(&guiLock_);
}

void InGameState::attackingCard(const Card &c)
{
    // Lock
    pthread_mutex_lock(&guiLock_);
    // Update
    attackingCards_.push_back(GUICard::create(c));
    dirtyPlayers();
    // Unlock
    pthread_mutex_unlock(&guiLock_);
    wait(400);
}

void InGameState::defendingCard(const Card &c)
{
    // Lock
    pthread_mutex_lock(&guiLock_);
    // Update
    defendingCards_.push_back(GUICard::create(c));
    dirtyPlayers();
    // Unlock
    pthread_mutex_unlock(&guiLock_);

    wait(400);
}

void InGameState::piledOnCard(const Card &c)
{
    // Lock
    pthread_mutex_lock(&guiLock_);
    // Update
    attackingCards_.push_back(GUICard::create(c));
    dirtyPlayers();
    // Unlock
    pthread_mutex_unlock(&guiLock_);
    wait(400);
}

void InGameState::playedOut(ConstPlayerPtr player)
{
    pthread_mutex_lock(&guiLock_);
    /* Empty */
    pthread_mutex_unlock(&guiLock_);
}

void InGameState::givenCards(ConstPlayerPtr player, int numCards)
{
    pthread_mutex_lock(&guiLock_);
    // Freeze
    // Animation
    // Need update
    dirtyPlayers();
    pthread_mutex_unlock(&guiLock_);
}

void InGameState::givenCards(ConstPlayerPtr player, const std::vector<Card>& cards)
{
    pthread_mutex_lock(&guiLock_);
    // Freeze
    // Animation
    // Need update
    dirtyPlayers();
    pthread_mutex_unlock(&guiLock_);
}

void InGameState::wait(int ms)
{
    SDL_Delay(ms);
}


void InGameState::drawPlayedCards()
{
    glTranslatef(GUIApp::SCREENX/2, GUIApp::SCREENY/2, 0);
    glTranslatef(-1.*GUICard::CARDX - 0.4*GUICard::CARDX,
                 -.6*GUICard::CARDY, 0);
    for (int i = 0; i < attackingCards_.size(); i++)
    {
        // Draw attacking Card
        attackingCards_[i]->draw();
        // Move over for defending card
        glTranslatef(GUICard::CARDX * 0.2, 0, 0);
        // Draw defending card if it exists
        if (defendingCards_.size() > i)
            defendingCards_[i]->draw();

        // Move over for next set
        if (i == 2)
            glTranslatef(-2*(GUICard::CARDX * 1.2) - 3*GUICard::CARDX*0.2,
                         1.2 * GUICard::CARDY, 0);
        else
            glTranslatef(GUICard::CARDX * 1.2, 0, 0);
    }
}

void InGameState::drawPiles()
{
    if (!validSizes_)
    {
        std::stringstream deckss, discardss;
        deckss << deckSize_;
        discardss << discardSize_;

        deckString_ = GUIString::create(deckss.str());
        discardString_ = GUIString::create(discardss.str());
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
        trumpCard_->draw();
        glPopMatrix();
    }
    if (deckSize_ > 1)
    {
        GUICard::drawCardBack();
        glColor3f(0, 0, 0);
        deckString_->draw();
    }

    if (discardSize_ > 0)
    {
        glLoadIdentity();
        glTranslatef(GUIApp::SCREENX - 10 - GUICard::CARDX/2, 10 + GUICard::CARDY/2, 0);
        glColor3f(1, 1, 1);
        GUICard::drawCardBack();
        glColor3f(0, 0, 0);
        discardString_->draw();
    }
}

void InGameState::drawPlayers()
{
    updatePlayers();
    float angle = M_PI/2;
    for (int i = 0; i < players_.size(); i++)
    {
        float x = cos(angle);
        float y = sin(angle);
        const float rx = GUIApp::SCREENX/2 - GUICard::CARDY/2 - 5;
        const float ry = GUIApp::SCREENY/2 - GUICard::CARDY/2 - 5;

        // Center
        glLoadIdentity();
        glTranslatef(GUIApp::SCREENX/2, GUIApp::SCREENY/2, 0);
        // Move outwards
        glTranslatef(x*rx, y*ry, 0);
        // Rotate
        glRotatef(angle*180/M_PI - 90, 0, 0, 1);

        // Draw the player
        playersDisplay_[i]->draw();

        angle += 2*M_PI/ players_.size();
    }
    // Unlock
    pthread_mutex_unlock(&guiLock_);
}

void InGameState::updatePlayers()
{
    // Update the name textures, first delete the old ones
    if (!validPlayerDisplays_)
    {
        for (int i = 0; i < playersDisplay_.size(); i++)
            delete playersDisplay_[i];
        playersDisplay_.resize(players_.size());

        humanView_ = new GUIHumanView((GUIPlayer *) players_[0].get());
        playersDisplay_[0] = humanView_;
        for (int i = 1; i < players_.size(); i++)
            playersDisplay_[i] = new GUIPlayerView(players_[i].get());
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
}

void InGameState::dirtyPlayers()
{
    for (int i = 0; i < playersDisplay_.size(); i++)
        playersDisplay_[i]->dirty();
}


void InGameState::setPlayers(const vector<PlayerPtr>& players)
{
    // Update
    players_ = players;
    validPlayerDisplays_ = false;

    vector<PlayerPtr>::iterator it;
    for (it = players_.begin(); it != players_.end(); it++)
    {
        if ((*it)->getName() == "guiplayer")
        {
            std::rotate(players_.begin(), it, players_.end());
            break;
        }
    }
}
