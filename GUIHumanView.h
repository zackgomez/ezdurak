#pragma once
#include "GUIPlayerView.h"

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

};
