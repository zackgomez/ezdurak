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

#ifndef M_PI
#define M_PI 3.141592653589793238462643
#endif

const static float DECK_X = 15 + GUICard::CARDX/2.;
const static float DECK_Y = 15 + GUICard::CARDY/2.;
const static float DISCARD_X = GUIApp::SCREENX - 15 - GUICard::CARDX/2.;
const static float DISCARD_Y = 15 + GUICard::CARDY/2.;

using namespace std;

void* game_thread_main(void *gameobj)
{
    srand(time(NULL)); Game *game = (Game*) gameobj;
    game->run();

    return NULL;
}

GUIStatePtr InGameState::create(int numPlayers)
{
    GUIStatePtr ret(new InGameState(numPlayers));
    return ret;
}

InGameState::InGameState(int numPlayers) :
    gameOver_(false),
    validPlayerDisplays_(true),
    humanView_(NULL)
{
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
    gameThread_.run(game_thread_main, game);
}

InGameState::~InGameState()
{
    animations_.clear();
    queue_.killReader();
    gameThread_.join();
    delete game;

    for (int i = 0; i < playersDisplay_.size(); i++)
        delete playersDisplay_[i];
}

void InGameState::render()
{
    // Clear
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Lock
    Lock l(guiLock_);

    drawPlayedCards();
    drawPiles();
    drawAnimations();
    drawPlayers();

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

void InGameState::gameStart(GameAgent *agent)
{
    Lock l(guiLock_);

    // First set the trumpCard in case we need it later
    trumpCard_ = agent_->getTrumpCard();

    // Set the deck and discard size
    deck_ = PileCardHolder(agent_->getDeckSize());
    discard_ = PileCardHolder(agent_->getDiscardSize());

    // Now set the local players
    const vector<PlayerPtr> players = agent_->getPlayers();
    setPlayers(players);

    // Now we wait for the displays to be created in another thread
    syncCond_.wait(guiLock_);
}

void InGameState::gameOver(ConstPlayerPtr biscuit)
{
    Lock l(guiLock_);
    assert(players_.size() == playersDisplay_.size());

    attacker_ = PlayerPtr();
    defender_ = PlayerPtr();
    biscuit_ = biscuit;
    //animations_.push_back(DelayAnimation::create(30));
    animations_.push_back(SetAnimation::create(gameOver_));

}

void InGameState::newRound(ConstPlayerPtr attacker, ConstPlayerPtr defender)
{
    Lock l(guiLock_);
    assert(players_.size() == playersDisplay_.size());
    
    playedCards_.clearNextLocation();

    animations_.push_back(StatusChangeAnimation::create(playerDisplayMap_[attacker_], GUIPlayerView::NONE,
                                                        playerDisplayMap_[defender_], GUIPlayerView::NONE));
    animations_.push_back(StatusChangeAnimation::create(playerDisplayMap_[attacker], GUIPlayerView::ATTACKER,
                                                        playerDisplayMap_[defender], GUIPlayerView::DEFENDER));
    animations_.push_back(DelayAnimation::create(20));
    attacker_ = attacker;
    defender_ = defender;
}

void InGameState::attackerPassed(ConstPlayerPtr newAttacker)
{
    Lock l(guiLock_);
    assert(players_.size() == playersDisplay_.size());
    
    animations_.push_back(StatusChangeAnimation::create(playerDisplayMap_[attacker_], GUIPlayerView::NONE,
                                                        playerDisplayMap_[newAttacker], GUIPlayerView::ATTACKER));
    animations_.push_back(DelayAnimation::create(20));
    attacker_ = newAttacker;
}

void InGameState::endRound(bool successfulDefend)
{
    Lock l(guiLock_);
    assert(players_.size() == playersDisplay_.size());

    if (successfulDefend)
    {
        // Get rid of the current attacker
        animations_.push_back(StatusChangeAnimation::create(playerDisplayMap_[attacker_],
                                                           GUIPlayerView::NONE));

        // Now add an animation crunching all of the cards to the discard pile
        animations_.push_back(CrunchAnimation::create(&playedCards_, &discard_, 25, DISCARD_X, DISCARD_Y));
    }
    else
    {
        animations_.push_back(StatusChangeAnimation::create(playerDisplayMap_[defender_],
                                                            GUIPlayerView::DEFENDERLOST));
    }
}

void InGameState::attackingCard(const Card &c)
{
    // Lock
    Lock l(guiLock_);
    assert(players_.size() == playersDisplay_.size());

    float x0, y0, angle, x1, y1;
    getPlayerPosition(playerPositionMap_[attacker_], x0, y0, angle);
    playedCards_.getNextCardLocation(true, x1, y1);
    animations_.push_back(MoveAnimation::create(c, playerDisplayMap_[attacker_]->getCardHolder(), playedCards_.getAttackingHolder(),
                                                25, x0, y0, x1, y1));
}

void InGameState::defendingCard(const Card &c)
{
    // Lock
    Lock l(guiLock_);
    assert(players_.size() == playersDisplay_.size());

    float x0, y0, angle, x1, y1;
    getPlayerPosition(playerPositionMap_[defender_], x0, y0, angle);
    playedCards_.getNextCardLocation(false, x1, y1);
    animations_.push_back(MoveAnimation::create(c, playerDisplayMap_[defender_]->getCardHolder(), playedCards_.getDefendingHolder(),
                                                25, x0, y0, x1, y1));
}

void InGameState::piledOnCard(const Card &c)
{
    // Lock
    Lock l(guiLock_);
    assert(players_.size() == playersDisplay_.size());

    float x0, y0, angle, x1, y1;
    getPlayerPosition(playerPositionMap_[attacker_], x0, y0, angle);
    playedCards_.getNextCardLocation(true, x1, y1);
    animations_.push_back(MoveAnimation::create(c, playerDisplayMap_[attacker_]->getCardHolder(), playedCards_.getAttackingHolder(),
                                                25, x0, y0, x1, y1));
}

void InGameState::playedOut(ConstPlayerPtr player)
{
    Lock l(guiLock_);
    assert(players_.size() == playersDisplay_.size());

    // TODO some kind of animation here (v2.0)
}

void InGameState::givenCards(ConstPlayerPtr player, int numCards)
{
    Lock l(guiLock_);
    assert(players_.size() == playersDisplay_.size());

    float x, y, angle;
    getPlayerPosition(playerPositionMap_[player], x, y, angle);
    for (int i = 0; i < numCards; i++)
        animations_.push_back(MoveAnimation::create(Card(), &deck_, playerDisplayMap_[player]->getCardHolder(),
                                                    25, DECK_X, DECK_Y, x, y));
}

void InGameState::givenCards(ConstPlayerPtr player, const std::vector<Card>& cards)
{
    Lock l(guiLock_);
    assert(players_.size() == playersDisplay_.size());

    // Get rid of the current attacker
    animations_.push_back(StatusChangeAnimation::create(playerDisplayMap_[attacker_],
                                                        GUIPlayerView::NONE));

    // TODO change this to another crunch animation
    float x, y, angle;
    getPlayerPosition(playerPositionMap_[player], x, y, angle);
    animations_.push_back(CrunchAnimation::create(&playedCards_, cards, playerDisplayMap_[player]->getCardHolder(),
        25, x, y));
}

void InGameState::drawPlayedCards() {
    glTranslatef(GUIApp::SCREENX/2, GUIApp::SCREENY/2, 0);
    playedCards_.render();
}

void InGameState::drawPiles()
{
    // Draw the deck and discard pile
    glLoadIdentity();
    glTranslatef(DECK_X, DECK_Y, 0);
    glColor3f(1, 1, 1);
    // First draw the trump, rotated and moved
    if (deck_.getNumCards() > 0)
    {
        glPushMatrix();
        glTranslatef(GUICard::CARDY/2 - GUICard::CARDX/2, 0, 0);
        glRotatef(90, 0, 0, 1);
        GUICard::draw(trumpCard_);
        glPopMatrix();
    }
    if (deck_.getNumCards() > 1)
    {
        GUICard::drawCardBack();
        glColor3f(0, 0, 0);
        //deckString_->draw();
    }

    if (discard_.getNumCards() > 0)
    {
        glLoadIdentity();
        glTranslatef(DISCARD_X, DISCARD_Y, 0);
        glColor3f(1, 1, 1);
        GUICard::drawCardBack();
        glColor3f(0, 0, 0);
        //discardString_->draw();
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
        syncCond_.signal();
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
