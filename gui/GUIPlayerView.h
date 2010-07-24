#pragma once
#include <vector>
#include <queue>
#include "GUIString.h"
#include "GUICard.h"
#include "Animation.h"

class Player;

class GUIPlayerView
{
public:
    enum Status {NONE, ATTACKER, DEFENDER};
    static GLuint attackEmblem, defendEmblem;

    GUIPlayerView(const Player *player);
    virtual ~GUIPlayerView();

    virtual void draw();
    virtual void dirty();
    virtual void setStatus(Status status);
    virtual void addAnimation(AnimationPtr a);

protected:
    const Player *player_;

    std::queue<AnimationPtr> animations_;
    std::vector<GUICardPtr> cards_;
    GUIStringPtr name_;
    Status status_;
    bool dirty_;


    virtual void drawName();
    virtual void drawCards();
    virtual void drawAnimations();
    virtual void update();

    void drawEmblem(GLuint tex);
};

