#pragma once
#include "GUIState.h"
#include "core/GameListener.h"
#include <vector>
#include <list>
#include <map>
#include "core/Card.h"
#include "core/Player.h"
#include "util/Thread.h"
#include "util/SynchronizedQueue.h"
#include "PlayedCardsView.h"
#include "PileCardHolder.h"
#include "Animation.h"
#include "GUIPlayer.h"
#include "core/Game.h"

class GUIListener;
class GUIPlayerView;
class GUIHumanView;

/** 
 * GUIApp state for actually playing an EZDurak game.
 */
class InGameState :
    public GUIState,
    public GameListener
{
public:
    /** 
     * Creates an InGameState ready for use.  The InGameState takes responsibility
     * for the passed Game object and will delete it when it is done.  The 
     * passed Game object should have all the players already included except 
     * for the GUIPlayer that the InGameState will add.
     * 
     * @param game The game object, ready for running save the GUIPlayer.
     * 
     * @return An InGameState ready for use.
     */
    static GUIStatePtr create(GamePtr game);

    /// Destructor
    virtual ~InGameState();

    // Functions inherited from GUIState Interface
    virtual void render();
    virtual void processEvent(SDL_Event &e);
    virtual bool needsTransition() const;
    virtual GUIStatePtr nextState();

    // Functions inherited from GameListener Interface
    virtual void gameStart(GameAgent *);
    virtual void gameOver(ConstPlayerPtr firstOut, ConstPlayerPtr biscuit);
    virtual void newRound(ConstPlayerPtr attacker, ConstPlayerPtr defender);
    virtual void attackerPassed(ConstPlayerPtr newAttacker);
    virtual void endRound(bool successfulDefend);
    virtual void attackingCard(const Card &c);
    virtual void defendingCard(const Card &c);
    virtual void piledOnCard(const Card &c);
    virtual void playedOut(ConstPlayerPtr player);
    virtual void givenCards(ConstPlayerPtr player, int numCard);
    virtual void givenCards(ConstPlayerPtr player, const std::vector<Card>& cards);

private:
    // Private constructors for create idiom
    InGameState(GamePtr game);
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

    // Game Player members
    GUIPlayerPtr guiPlayer_;
    std::vector<PlayerPtr> players_;
    ConstPlayerPtr attacker_, defender_;
    ConstPlayerPtr biscuit_, firstOut_;
    bool gameOver_;

    // Player display members
    bool validPlayerDisplays_;
    GUIHumanView *humanView_;
    std::vector<GUIPlayerView*> playersDisplay_;
    std::map<ConstPlayerPtr, GUIPlayerView*> playerDisplayMap_;
    std::map<ConstPlayerPtr, int> playerPositionMap_;

    // Pile display members
    PileCardHolder deck_;
    PileCardHolder discard_;

    // Game members
    GamePtr game_;
    GameAgent *agent_;
    SynchronizedQueue<int> queue_;
    Thread gameThread_;
    Mutex guiLock_;
    CondVar syncCond_;;
};
