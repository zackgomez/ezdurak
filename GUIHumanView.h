#pragma once
#include "GUIPlayerView.h"
#include "GUIString.h"

class GUIPlayer;

class GUIHumanView :
    public GUIPlayerView
{
public:
    GUIHumanView(const GUIPlayer *player);
    ~GUIHumanView();

    virtual void draw();

private:
    const GUIPlayer *player_;
    void drawCards();
    GUIString passString_;
    GUIString giveUpString_;
};
