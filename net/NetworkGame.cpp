#include "NetworkGame.h"
#include <iostream>
#include "core/Game.h"
#include "core/GameListener.h"
#include "NetworkProtocol.h"

using std::string;

struct game_thread_arg
{
    SynchronizedQueue<Message> *queue;
    kissnet::tcp_socket_ptr sock;
};

static bool network_running;
void * game_thread(void *arg)
{
    struct game_thread_arg *args = (struct game_thread_arg *) arg;
    SynchronizedQueue<Message> *queue = args->queue;
    kissnet::tcp_socket_ptr sock = args->sock;

    // TODO send handshake

    char header[3];
    // Now read from the socket and push messages on to the queue
    while (network_running)
    {
        // First read the header
        sock->recv(header, 3);
        // Calculate payload size, lsb first in header, and message type
        int payload_size = header[0] + 256 * header[1];
        char type = header[2];

        char *raw_payload = new char[payload_size];
        sock->recv(raw_payload, payload_size);

        Message m;
        m.type = type;
        m.payload = string(raw_payload, payload_size);



        queue->enqueue(m);
    }

    return NULL;
}

NetworkGame::NetworkGame() :
    connected_(false)
{
    network_running = false;
    sock_ = kissnet::tcp_socket::create();
    /* Empty */
}
NetworkGame::~NetworkGame()
{
    if (connected_)
        sock_->close();

    if (network_running)
    {
        network_running = false;
        queue_.killReader();
        game_thread_.join();
    }
}

void NetworkGame::run()
{
    // Start the game thread
    network_running = true;
    struct game_thread_arg arg;
    arg.queue = &queue_;
    arg.sock  = sock_;
    game_thread_.run(game_thread, &arg);

    // Sit in a loop and read messages from the message queue, acting upon them
    // as they come in.
    Message m;
    PlayerPtr p;
    ConstPlayerPtr cp;
    Card c;
    std::vector<Card> cs;
    bool b;
    char n;
    for (;;)
    {
        m = queue_.dequeue();
        string payload = m.payload;

        switch(m.type)
        {
        case MSG_GAMESTARTING:
            // TODO
            // Read trump, player name array
            // Build players array, and give to super class
            // Set deckSize
            // Broadcast
            break;
        case MSG_GAMEOVER:
            cp = readPlayer(payload, players_);
            // Broadcast, kill thread, exit
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->gameOver(p);
            network_running = false;
            return;
            break;
        case MSG_NEWROUND:
            assert(payload.size() == 2);
            cp = readPlayer(payload, players_);
            attacker_ = p;
            cp = readPlayer(string(payload.c_str()[1], 1), players_);
            defender_ = p;
            playedCards_.clear();
            tricksLeft_ = std::min(defender_->getNumCards(), Game::HAND_SIZE);
            // Set attacker and defender, clear played cards and set cardsLeft
            // Broadcast
            break;
        case MSG_ATTACKERPASSED:
            cp = readPlayer(m.payload, players_);
            // Set new attacker
            attacker_ = p;
            // Broadcast
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->attackerPassed(p);
            break;
        case MSG_ENDROUND:
            b = readBool(m.payload);
            // On successful defend, add to discard size
            if (b)
                discardSize_ += playedCards_.size();
            // Broadcast
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->endRound(b);
            break;
        case MSG_ATTACKINGCARD:
            // TODO
            // Remove card from attacker_
            // Add to played cards & playableRanks
            // update tricksLeft
            // Broadcast
            break;
        case MSG_DEFENDINGCARD:
            // TODO
            // Remove card from defender_
            // Add to played cards & playableRanks
            // Broadcast
            break;
        case MSG_PILEDONCARD:
            // TODO
            // Remove card from attacker_
            // Add to played cards & playableRanks
            // update tricksLeft
            // Broadcast
            break;
        case MSG_PLAYEDOUT:
            assert (m.payload.size() == 1);
            cp = readPlayer(m.payload, players_);
            // Broadcast
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->playedOut(p);
            break;
        case MSG_GIVENCARDSN:
            assert(payload.size() == 2);
            p = readPlayer(m.payload, players_);
            n = m.payload[1];
            // Remove cards n from the deck
            assert(deckSize_ >= n);
            deckSize_ -= n;
            // Add n cards to player
            cs.resize(n);
            p->addCards(cs);
            // Broadcast
            break;
        case MSG_GIVENCARDSCS:
            // TODO
            // Add cs.size cards to player
            // Broadcast
            break;
        default:
            std::cerr << "NetworkGame: Unknown message type\n";
        }
    }

}

bool NetworkGame::connectTo(const std::string &host, const std::string &port)
{
    try
    {
        sock_->connect(host, port);
        // TODO handshake
        connected_ = true;
    }
    catch (kissnet::socket_exception &e)
    {
        std::cerr << "Unable to connect to " << host << ":" << port << ' '
            << e.what() << '\n';
        return false;
    }

    std::cout << "Successfully connected\n";

    return true;
}

int NetworkGame::getDeckSize() const
{
    return deckSize_;
}

int NetworkGame::getDiscardSize() const
{
    return discardSize_;
}

