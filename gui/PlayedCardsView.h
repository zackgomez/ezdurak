#pragma once
#include "CardHolderImpl.h"
#include "Animation.h"

/** 
 * View for the cards that are currently on the table in the GUI.
 * Has two CardHolders, one for the attacking and defending cards played.
 * Has some convienence methods for animations.
 */
class PlayedCardsView
{
public:
    PlayedCardsView();

    /** 
     * @return The attacking cards CardHolder
     */
    CardHolder* getAttackingHolder();
    /** 
     * @return The defending cards CardHolder
     */
    CardHolder* getDefendingHolder();

    /** 
     * Removes all the cards from the attacking and defending CardHolders.
     */
    void clear();
    /** 
     * Draws the cards to the window at the current position.
     */
    void render();

    /** 
     * Convienence method for getting the location of the next card to be added.
     * Each call moves the next card forward and should be cleared by 
     * clearNextLocation before being queried for animations for a new round.
     * 
     * @param attacking True if it would be an attacking card.
     * @param x Out param
     * @param y Out param
     */
    void getNextCardLocation(bool attacking, float &x, float &y);
    /** 
     * Clears the next location returned by getNextCardLocation.
     */
    void clearNextLocation();

    /** 
     * Returns a crunch animation moving the list of the cards to another CardHolder
     * 
     * @param cs The cards to move
     * @param target Destination CardHolder
     * @param dur The number of frames the CrunchAnimation should take
     * @param x1 Destination x coord
     * @param y1 Destination y coord
     * 
     * @return CrunchAnimation
     */
    AnimationPtr getAnimation(const std::vector<Card> &cs, CardHolder *target,
                              int dur, float x1, float y1);
    /** 
     * Returns a crunch animation moving all of the cards to another CardHolder.
     * 
     * @param target The destination CardHolder
     * @param dur The number of frames the Animation should take
     * @param x1 Destination x coord
     * @param y1 Destination y coord
     * 
     * @return CrunchAnimation
     */
    AnimationPtr getAnimation(CardHolder *target, int dur, float x1, float y1);
    
private:
    // Helper functions
    void getCardLocation(bool attacking, int index, float &x, float &y);
    AnimationPtr getAnimation(const Card &c, CardHolder *target,
                              int dur, float x1, float y1);

    // Data members
    CardHolderImpl attackingCards_;
    CardHolderImpl defendingCards_;

    int nextAttackingIdx_;
    int nextDefendingIdx_;
};

