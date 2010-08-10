#pragma once
#include <vector>
#include <queue>
#include "GUIString.h"
#include "CardHolder.h"

class Player;

class GUIPlayerView
{
public:
    enum Status {NONE, ATTACKER, DEFENDER};
    static GLuint attackEmblem, defendEmblem;

    GUIPlayerView(const Player *player);
    virtual ~GUIPlayerView();

    virtual void draw();
    virtual void setStatus(Status status);

    // Functions inherited from CardHolder Interface
    virtual void addCard(Card c);
    virtual void addCards(const std::vector<Card>& cs);
    virtual void removeCard(Card c);

protected:
    const Player *player_;

    int numCards_;
    GUIStringPtr name_;
    Status status_;

    virtual void drawName();
    virtual void drawCards();

    void drawEmblem(GLuint tex);
};

