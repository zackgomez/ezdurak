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

    virtual void draw();


private:
    const GUIPlayer *player_;
    SynchronizedQueue<int> &queue_;
    void drawCards();
    GUIStringPtr passString_;
    GUIStringPtr giveUpString_;
};
