#pragma once

class Player;
class GUIString;

class GUIPlayer
{
public:
    enum Status {NONE, ATTACKER, DEFENDER};
    GUIPlayer(const Player *player);
    ~GUIPlayer();

    void draw();
    void setStatus(Status status);

private:
    const Player *player_;
    GUIString *name_;
    Status status_;
};

