#pragma once
#include "GUIPlayerView.h"
#include "GUIString.h"
#include "SynchronizedQueue.h"
#include "HumanCardHolder.h"

class GUIPlayer;

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
    virtual void drawCards();

    HumanCardHolder cards_;
    const GUIPlayer *player_;

    SynchronizedQueue<int> &queue_;

    GUIStringPtr passString_;
    GUIStringPtr giveUpString_;
};
