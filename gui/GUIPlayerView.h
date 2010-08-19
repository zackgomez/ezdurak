#pragma once
#include <vector>
#include <queue>
#include "GUIString.h"
#include "CardHolderImpl.h"

class Player;

class GUIPlayerView
{
public:
    enum Status {NONE, ATTACKER, DEFENDER, DEFENDERLOST};
    static GLuint attackEmblem, defendEmblem, defendLostEmblem;
    static const int EMBLEM_SIZE;

    GUIPlayerView(const Player *player);
    virtual ~GUIPlayerView();

    virtual void draw();
    virtual void setStatus(Status status);

    virtual CardHolder * getCardHolder();

protected:
    const Player *player_;

    CardHolderImpl cards_;

    GUIStringPtr name_;
    Status status_;

    virtual void drawName();
    virtual void drawCards();

    void drawEmblem(GLuint tex);
};

