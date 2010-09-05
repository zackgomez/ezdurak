#pragma once
#include "GUIPlayerView.h"
#include "GUIString.h"
#include "util/SynchronizedQueue.h"
#include "HumanCardHolder.h"

class GUIPlayer;

/** 
 * An implementation of the GUIPlayerView for the user interacting with the GUI.
 * Has a method for handling mouse clicks and also draws the front of cards 
 * instead of the backs.
 */
class GUIHumanView :
    public GUIPlayerView
{
public:
    GUIHumanView(const GUIPlayer *player);
    ~GUIHumanView();

    void mouseClick(int x, int y);

    virtual CardHolder * getCardHolder();

private:
    // Inherited from GUIPlayerView
    virtual void drawCards(bool animating);

    HumanCardHolder cards_;
    const GUIPlayer *player_;

    SynchronizedQueue<int> &queue_;

    GUIStringPtr passString_;
    GUIStringPtr giveUpString_;
};
