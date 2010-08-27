#pragma once
#include "core/Game.h"
#include <set>
#include "kissnet.h"
#include "util/Thread.h"
#include "util/SynchronizedQueue.h"

struct Message
{
    char type;
    std::string payload;
};

class NetworkGame :
    public Game
{
public:
    NetworkGame();
    ~NetworkGame();

    bool connectTo(const std::string &host, const std::string &port);

    // Functions overriden from Game interface
    virtual void run();

    // Functions overriden from GameAgent interface
    virtual int getDeckSize() const;
    virtual int getDiscardSize() const;

private:
    kissnet::tcp_socket_ptr sock_;
    bool connected_;

    Thread game_thread_;
    SynchronizedQueue<Message> queue_;

    int deckSize_;
    int discardSize_;
};

