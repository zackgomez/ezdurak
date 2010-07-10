#pragma once

class Player;
class GUIString;

class GUIPlayerView
{
public:
    enum Status {NONE, ATTACKER, DEFENDER};
    GUIPlayerView(const Player *player);
    virtual ~GUIPlayerView();

    virtual void draw();
    virtual void setStatus(Status status);

protected:
    const Player *player_;
    GUIString *name_;
    Status status_;

    void drawName();
    void drawCards();
};

