#pragma once
#include "GUIState.h"
#include "core/GameListener.h"
#include <vector>
#include <list>
#include <map>
#include <pthread.h>
#include "GUIString.h"
#include "core/Card.h"
#include "core/Player.h"
#include "SynchronizedQueue.h"
#include "PlayedCardsView.h"
#include "Animation.h"

class Game;
class GUIListener;
class GUIPlayerView;
class GUIHumanView;

class InGameState :
    public GUIState,
    public GameListener
{
public:
    static GUIStatePtr create(int numPlayers);

    /// Destructor
    virtual ~InGameState();

    // Functions inherited from GUIState Interface
    virtual void render();
    virtual void processEvent(SDL_Event &e);
    virtual bool needsTransition() const;
    virtual GUIStatePtr nextState();

    // Functions inherited from GameListener Interface
    void gameStart();
    void gameOver(ConstPlayerPtr biscuit);
    void newRound(ConstPlayerPtr attacker, ConstPlayerPtr defender);
    void attackerPassed(ConstPlayerPtr newAttacker);
    void endRound(bool successfulDefend);
    void attackingCard(const Card &c);
    void defendingCard(const Card &c);
    void piledOnCard(const Card &c);
    void playedOut(ConstPlayerPtr player);
    void givenCards(ConstPlayerPtr player, int numCard);
    void givenCards(ConstPlayerPtr player, const std::vector<Card>& cards);
    void wait(int ms);

private:
    // Private constructors for create idiom
    InGameState(int numPlayers);
    InGameState(const InGameState&);
    InGameState& operator=(const InGameState &);

    // Rendering Helper functions
    void drawPlayedCards();
    void drawPiles();
    void drawPlayers();
    void drawAnimations();
    void updatePlayers();
    void getPlayerPosition(int i, float &x, float &y, float &a);

    // GameListener helper functions
    void setPlayers(const std::vector<PlayerPtr>& players);
    void setTrumpCard(const Card &c);

    // --- Data members ---
    // Contains the next state, if known, or null
    GUIStatePtr next_;

    // Card related members
    std::list<AnimationPtr> animations_;
    PlayedCardsView playedCards_;
    Card trumpCard_;
    int deckSize_;
    int discardSize_;

    // Game Player members
    std::vector<PlayerPtr> players_;
    ConstPlayerPtr attacker_, defender_;
    ConstPlayerPtr biscuit_;
    bool gameOver_;

    // Player display members
    bool validPlayerDisplays_;
    // TODO:2010-07-19:zack: Make this a smart ptr (probably auto_ptr)
    GUIHumanView *humanView_;
    std::vector<GUIPlayerView*> playersDisplay_;
    std::map<ConstPlayerPtr, GUIPlayerView*> playerDisplayMap_;
    std::map<ConstPlayerPtr, int> playerPositionMap_;
    bool validStatus_;

    // Pile display members
    bool validSizes_;
    GUIStringPtr deckString_;
    GUIStringPtr discardString_;

    // Game members
    Game *game;
    GameAgent *agent_;
    pthread_t game_thread;
    SynchronizedQueue<int> queue_;
    pthread_mutex_t guiLock_;
    pthread_cond_t  displaysCV_;
};
