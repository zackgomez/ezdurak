#include "NetworkGame.h"
#include <iostream>
#include "core/Game.h"
#include "core/GameListener.h"
#include "NetworkProtocol.h"
#include "ProxyPlayer.h"

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
    // Send ready handshake
    {
        std::string handshake = createMessage(MSG_READY, "");
        sock->send(handshake);
    }

    char header[3];
    // Now read from the socket and push messages on to the queue
    while (network_running)
    {
        // First read the header
        int bytes_recieved = sock->recv(header, 3);
        if (bytes_recieved < 3)
        {
            network_running = false;
            std::cout << "unable to read full header, quitting\n";

            // Push an exit message so that the other thread knows...
            Message m;
            m.type = MSG_END;
            queue->enqueue(m);
            continue;
        }
        // Calculate payload size, lsb first in header, and message type
        int payload_size = header[0] + 256 * header[1];
        char type = header[2];

        char *raw_payload = new char[payload_size];
        bytes_recieved = sock->recv(raw_payload, payload_size);
        if (bytes_recieved != payload_size)
            std::cerr << "Unable to read entire payload in one shot.\n";
        Message m;
        m.type = type;
        m.payload = string(raw_payload, payload_size);

        if (m.type == MSG_END)
            network_running = false;

        queue->enqueue(m);
    }

    return NULL;
}

NetworkGame::NetworkGame() :
    connected_(false)
{
    network_running = false;
    sock_ = kissnet::tcp_socket::create();
}

NetworkGame::NetworkGame(PlayerPtr localPlayer) :
    localPlayer_(localPlayer)
{
    network_running = false;
    sock_ = kissnet::tcp_socket::create();
}

NetworkGame::~NetworkGame()
{
    if (connected_)
        sock_->close();

    if (network_running)
    {
        network_running = false;
        game_thread_.join();
    }
}

void NetworkGame::run()
{
    // TODO need to send MSG_NAME somewhere if localPlayer_ is set
    // Send the name as name#id where id is some unique identifier
    // Send before the game_thread starts
    if (localPlayer_.get())
    {
        string payload = serializeString(localPlayer_->getName() + "#NETP");
        sock_->send(createMessage(MSG_NAME, payload));
    }

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
    ProxyPlayer *pp;
    Card c;
    std::vector<Card> cs;
    std::string s;
    bool b;
    char n;
    int numPlayers;
    int stridx;
    for (;;)
    {
        m = queue_.dequeue();
        string payload = m.payload;

        switch(m.type)
        {
        case MSG_READY:
            std::cerr << "Got MSG_READY, should not have happened.\n";
            break;
        case MSG_END:
            std::cerr << "Got MSG_END, quitting...\n";
            network_running = false;
            return;
        case MSG_GAMESTARTING:
            // TESTED WORKS
            std::cerr << "Got MSG_GAMESTARTING\n";
            // Read trump, player name array
            trumpCard_ = readCard(payload);
            // Build players array, and give to super class
            numPlayers = payload[2];
            assert(numPlayers >= 2 && numPlayers <= 6);
            stridx = 3;
            for (int i = 0; i < numPlayers; i++)
            {
                // TODO check for the player who's name we set to name#id
                string name = readString(string(payload.c_str()+stridx));
                stridx += name.length() + 1;
                p = PlayerPtr(new ProxyPlayer(name));
                players_.push_back(p);
            }
            // Set deckSize
            deckSize_ = 36 - numPlayers * Game::HAND_SIZE;
            discardSize_ = 0;
            // Broadcast
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->gameStart();
            break;
        case MSG_GAMEOVER:
            // TESTED WORKS
            std::cerr << "Got MSG_GAMEOVER\n";
            assert(payload.size() == 1);
            cp = readPlayer(payload, players_);
            // Broadcast, kill thread, exit
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->gameOver(cp);
            network_running = false;
            return;
            break;
        case MSG_NEWROUND:
            std::cerr << "Got MSG_NEWROUND\n";
            assert(payload.size() == 2);
            // Set attacker
            p = readPlayer(payload, players_);
            attacker_ = p;
            // Set defender
            s = ""; s.push_back(payload[1]);
            p = readPlayer(s, players_);
            defender_ = p;
            // Reset variables
            playedCards_.clear();
            tricksLeft_ = std::min(defender_->getNumCards(), Game::HAND_SIZE);
            // Broadcast
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->newRound(attacker_, defender_);
            break;
        case MSG_ENDROUND:
            std::cerr << "Got MSG_ENDROUND\n";
            assert(payload.size() == 1);
            b = readBool(m.payload);
            // On successful defend, add to discard size
            if (b)
                discardSize_ += playedCards_.size();
            // Broadcast
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->endRound(b);
            break;
        case MSG_ATTACKERPASSED:
            std::cerr << "Got MSG_ATTACKERPASSED\n";
            assert(payload.size() == 1);
            p = readPlayer(m.payload, players_);
            // Set new attacker
            attacker_ = p;
            // Broadcast
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->attackerPassed(p);
            break;
        case MSG_ATTACKINGCARD:
            std::cerr << "Got MSG_ATTACKINGCARD\n";
            assert(payload.size() == 2);
            c = readCard(payload);
            // Remove card from attacker_
            pp = (ProxyPlayer *) attacker_.get();
            pp->removeCards(1);
            // Add to played cards & playableRanks
            playedCards_.push_back(c);
            playableRanks_.insert(c);
            // update tricksLeft
            tricksLeft_--;
            // Broadcast
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->attackingCard(c);
            break;
        case MSG_DEFENDINGCARD:
            std::cerr << "Got MSG_DEFENDINGCARD\n";
            assert(payload.size() == 2);
            // Broadcast
            std::cerr << "Got MSG_PILEDONCARD\n";
            assert(payload.size() == 2);
            c = readCard(payload);
            // Remove card from defender
            pp = (ProxyPlayer *) defender_.get();
            pp->removeCards(1);
            // Add to played cards & playableRanks
            playedCards_.push_back(c);
            playableRanks_.insert(c);
            // Broadcast
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->defendingCard(c);
            break;
        case MSG_PILEDONCARD:
            std::cerr << "Got MSG_PILEDONCARD\n";
            assert(payload.size() == 2);
            c = readCard(payload);
            // Remove card from attacker_
            pp = (ProxyPlayer *) attacker_.get();
            pp->removeCards(1);
            // Add to played cards & playableRanks
            playedCards_.push_back(c);
            playableRanks_.insert(c);
            // update tricksLeft
            tricksLeft_--;
            // Broadcast
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->piledOnCard(c);
            break;
        case MSG_PLAYEDOUT:
            std::cerr << "Got MSG_PLAYEDOUT\n";
            assert (m.payload.size() == 1);
            cp = readPlayer(m.payload, players_);
            // Broadcast
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->playedOut(cp);
            break;
        case MSG_GIVENCARDSN:
            std::cerr << "Got MSG_GIVENCARDSN\n";
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
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->givenCards(p, n);
            break;
        case MSG_GIVENCARDSCS:
            std::cerr << "Got MSG_GIVENCARDSCS\n";
            // Add cs.size cards to player
            p = readPlayer(m.payload, players_);
            cs = readCards(string(payload.c_str() + 1, payload.size() - 1));
            p->addCards(cs);
            // Broadcast
            for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
                (*lit_)->givenCards(p, cs);
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

