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
#include "MoveAnimation.h"
#include "ClearAnimation.h"
#include "StatusChangeAnimation.h"
#include "DelayAnimation.h"
#include "SetAnimation.h"

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
    deckSize_(0),
    discardSize_(0),
    gameOver_(false),
    validPlayerDisplays_(true),
    humanView_(NULL),
    validSizes_(false)
{
    pthread_mutex_init(&guiLock_, NULL);
    pthread_cond_init (&displaysCV_, NULL);

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
    animations_.clear();
    pthread_cancel(game_thread);
    pthread_join(game_thread, NULL);
    delete game;

    for (int i = 0; i < playersDisplay_.size(); i++)
        delete playersDisplay_[i];
    pthread_mutex_destroy(&guiLock_);
    pthread_cond_destroy (&displaysCV_);
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
    drawAnimations();
    drawPlayers();
    // Unlock
    pthread_mutex_unlock(&guiLock_);

}

bool InGameState::needsTransition() const
{
    return next_.get() || gameOver_;
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
        // If Animations in progress, do nothing
        if (!animations_.empty())
            break;
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

    // First set the trumpCard in case we need it later
    trumpCard_ = agent_->getTrumpCard();

    // Now set the local players
    const vector<PlayerPtr> players = agent_->getPlayers();
    setPlayers(players);

    // Now we wait for the displays to be created in another thread
    pthread_cond_wait(&displaysCV_, &guiLock_);

    pthread_mutex_unlock(&guiLock_);
}

void InGameState::gameOver(ConstPlayerPtr biscuit)
{
    pthread_mutex_lock(&guiLock_);
    assert(players_.size() == playersDisplay_.size());

    attacker_ = PlayerPtr();
    defender_ = PlayerPtr();
    biscuit_ = biscuit;
    animations_.push_back(DelayAnimation::create(30));
    animations_.push_back(SetAnimation::create(gameOver_));

    pthread_mutex_unlock(&guiLock_);
}

void InGameState::newRound(ConstPlayerPtr attacker, ConstPlayerPtr defender)
{
    pthread_mutex_lock(&guiLock_);
    assert(players_.size() == playersDisplay_.size());
    
    playedCards_.clearNextLocation();

    deckSize_ = agent_->getDeckSize();
    discardSize_ = agent_->getDiscardSize();

    animations_.push_back(StatusChangeAnimation::create(playerDisplayMap_[attacker_], GUIPlayerView::NONE,
                                                        playerDisplayMap_[defender_], GUIPlayerView::NONE));
    animations_.push_back(StatusChangeAnimation::create(playerDisplayMap_[attacker], GUIPlayerView::ATTACKER,
                                                        playerDisplayMap_[defender], GUIPlayerView::DEFENDER));
    animations_.push_back(DelayAnimation::create(20));
    attacker_ = attacker;
    defender_ = defender;

    pthread_mutex_unlock(&guiLock_);
}

void InGameState::attackerPassed(ConstPlayerPtr newAttacker)
{
    pthread_mutex_lock(&guiLock_);
    assert(players_.size() == playersDisplay_.size());
    
    animations_.push_back(StatusChangeAnimation::create(playerDisplayMap_[attacker_], GUIPlayerView::NONE,
                                                        playerDisplayMap_[newAttacker], GUIPlayerView::ATTACKER));
    animations_.push_back(DelayAnimation::create(20));
    attacker_ = newAttacker;

    pthread_mutex_unlock(&guiLock_);
}

void InGameState::endRound(bool successfulDefend)
{
    pthread_mutex_lock(&guiLock_);
    assert(players_.size() == playersDisplay_.size());

    if (successfulDefend)
    {
        // On successful defend, clear the play table
        animations_.push_back(DelayAnimation::create(30));
        animations_.push_back(ClearAnimation::create(playedCards_.getAttackingHolder()));
        animations_.push_back(ClearAnimation::create(playedCards_.getDefendingHolder()));
    }
    else
    {
        // TODO an animation for the defender losing
    }

    pthread_mutex_unlock(&guiLock_);
}

void InGameState::attackingCard(const Card &c)
{
    // Lock
    pthread_mutex_lock(&guiLock_);
    assert(players_.size() == playersDisplay_.size());

    float x0, y0, angle, x1, y1;
    getPlayerPosition(playerPositionMap_[attacker_], x0, y0, angle);
    playedCards_.getNextCardLocation(true, x1, y1);
    animations_.push_back(MoveAnimation::create(c, playerDisplayMap_[attacker_]->getCardHolder(), playedCards_.getAttackingHolder(),
                                                25, x0, y0, x1, y1));
    // Unlock
    pthread_mutex_unlock(&guiLock_);
}

void InGameState::defendingCard(const Card &c)
{
    // Lock
    pthread_mutex_lock(&guiLock_);
    assert(players_.size() == playersDisplay_.size());

    float x0, y0, angle, x1, y1;
    getPlayerPosition(playerPositionMap_[defender_], x0, y0, angle);
    playedCards_.getNextCardLocation(false, x1, y1);
    animations_.push_back(MoveAnimation::create(c, playerDisplayMap_[defender_]->getCardHolder(), playedCards_.getDefendingHolder(),
                                                25, x0, y0, x1, y1));
    // Unlock
    pthread_mutex_unlock(&guiLock_);
}

void InGameState::piledOnCard(const Card &c)
{
    // Lock
    pthread_mutex_lock(&guiLock_);
    assert(players_.size() == playersDisplay_.size());

    float x0, y0, angle, x1, y1;
    getPlayerPosition(playerPositionMap_[attacker_], x0, y0, angle);
    playedCards_.getNextCardLocation(true, x1, y1);
    animations_.push_back(MoveAnimation::create(c, playerDisplayMap_[attacker_]->getCardHolder(), playedCards_.getAttackingHolder(),
                                                25, x0, y0, x1, y1));
    // Unlock
    pthread_mutex_unlock(&guiLock_);
}

void InGameState::playedOut(ConstPlayerPtr player)
{
    pthread_mutex_lock(&guiLock_);
    assert(players_.size() == playersDisplay_.size());

    pthread_mutex_unlock(&guiLock_);
}

void InGameState::givenCards(ConstPlayerPtr player, int numCards)
{
    pthread_mutex_lock(&guiLock_);
    assert(players_.size() == playersDisplay_.size());

    float x, y, angle;
    getPlayerPosition(playerPositionMap_[player], x, y, angle);
    for (int i = 0; i < numCards; i++)
        animations_.push_back(MoveAnimation::create(Card(), NULL, playerDisplayMap_[player]->getCardHolder(),
                                                    25, GUIApp::SCREENX/2, GUIApp::SCREENY/2, x, y));
    pthread_mutex_unlock(&guiLock_);
}

void InGameState::givenCards(ConstPlayerPtr player, const std::vector<Card>& cards)
{
    pthread_mutex_lock(&guiLock_);
    assert(players_.size() == playersDisplay_.size());

    // TODO a better animation, with the cards "crunching"
    animations_.push_back(DelayAnimation::create(30));
    animations_.push_back(ClearAnimation::create(playedCards_.getAttackingHolder()));
    animations_.push_back(ClearAnimation::create(playedCards_.getDefendingHolder()));

    float x, y, angle;
    getPlayerPosition(playerPositionMap_[player], x, y, angle);
    for (int i = 0; i < cards.size(); i++)
        animations_.push_back(MoveAnimation::create(cards[i], NULL, playerDisplayMap_[player]->getCardHolder(),
                                                    25, GUIApp::SCREENX/2, GUIApp::SCREENY/2, x, y));
	
    pthread_mutex_unlock(&guiLock_);
}

void InGameState::wait(int ms)
{
    SDL_Delay(ms);
}


void InGameState::drawPlayedCards()
{
    glTranslatef(GUIApp::SCREENX/2, GUIApp::SCREENY/2, 0);
    playedCards_.render();
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
        GUICard::draw(trumpCard_);
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
    for (int i = 0; i < players_.size(); i++)
    {
        float testx, testy, testangle;
        getPlayerPosition(i, testx, testy, testangle);

        glLoadIdentity();
        glTranslatef(testx, testy, 0);
        glRotatef(testangle, 0, 0, 1);

        // Draw the player
        playersDisplay_[i]->draw();
    }
}

void InGameState::drawAnimations()
{
    if (!animations_.empty())
        animations_.front()->render();

    while (!animations_.empty() && animations_.front()->isDone())
        animations_.pop_front();
}

void InGameState::getPlayerPosition(int i, float& xout, float& yout, float& angleout)
{
    float angle = M_PI/2 + 2*M_PI*i/players_.size();
    float x = cos(angle);
    float y = sin(angle);
    const float rx = GUIApp::SCREENX/2 - GUICard::CARDY/2 - 5;
    const float ry = GUIApp::SCREENY/2 - GUICard::CARDY/2 - 5;

    xout = x*rx + GUIApp::SCREENX/2;
    yout = y*ry + GUIApp::SCREENY/2;
    angleout = angle * 180 / M_PI - 90;
}

void InGameState::updatePlayers()
{
    // Do we need to create the player displays?
    if (!validPlayerDisplays_)
    {
        // If animations are in progress, do nothing
        assert(animations_.empty());
        // Remove old displays
        for (int i = 0; i < playersDisplay_.size(); i++)
            delete playersDisplay_[i];
        playersDisplay_.resize(players_.size());

        humanView_ = new GUIHumanView((GUIPlayer *) players_[0].get());
        playersDisplay_[0] = humanView_;
        for (int i = 1; i < players_.size(); i++)
            playersDisplay_[i] = new GUIPlayerView(players_[i].get());

        // Create the map
        for (int i = 0; i < players_.size(); i++)
            playerDisplayMap_[players_[i]] = playersDisplay_[i];
        // We have created the players, signal to the other thread
        validPlayerDisplays_ = true;
        pthread_cond_signal(&displaysCV_);
    }
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

    for (int i = 0; i < players_.size(); i++)
        playerPositionMap_[players_[i]] = i;
}
