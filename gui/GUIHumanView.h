#pragma once
#include "GUIPlayerView.h"
#include "GUIString.h"
#include "SynchronizedQueue.h"

class GUIPlayer;

class GUIHumanView :
    public GUIPlayerView
{
public:
    GUIHumanView(const GUIPlayer *player);
    ~GUIHumanView();

    void mouseClick(int x, int y);

private:
    // Inherited from GUIPlayerView
    virtual void drawCards();

    const GUIPlayer *player_;
    std::vector<Card> cards_;

    SynchronizedQueue<int> &queue_;

    GUIStringPtr passString_;
    GUIStringPtr giveUpString_;
};
