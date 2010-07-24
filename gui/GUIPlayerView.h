#pragma once
#include <vector>
#include "GUIString.h"
#include "GUICard.h"

class Player;

class GUIPlayerView
{
public:
    enum Status {NONE, ATTACKER, DEFENDER};
    GUIPlayerView(const Player *player);
    virtual ~GUIPlayerView();

    virtual void draw();
    virtual void dirty();
    virtual void setStatus(Status status);

protected:
    const Player *player_;
    std::vector<GUICardPtr> cards_;
    GUIStringPtr name_;
    Status status_;
    bool dirty_;

    void drawName();
    void drawCards();
    void update();
};

