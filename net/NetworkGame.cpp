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
        assert (payload_size >= 0);
        char type = header[2];

        Message m;
        m.type = type;


        if (payload_size > 0)
        {
            char *raw_payload = new char[payload_size];
            bytes_recieved = sock->recv(raw_payload, payload_size);
            if (bytes_recieved != payload_size)
                std::cerr << "Unable to read entire payload in one shot.\n";
            m.payload = string(raw_payload, payload_size);
            delete raw_payload;
        }
        else
            m.payload = "";

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
    // Send the name as name#id where id is some unique identifier
    // Send before the game_thread starts
    if (localPlayer_.get())
    {
        // TODO make a unique ID, using a random number or something
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
    running_ = true;
    while (running_)
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
            endMessage(payload);
            break;
        case MSG_GAMESTARTING:
            std::cerr << "Got MSG_GAMESTARTING\n";
            gameStartingMessage(payload);
            break;
        case MSG_GAMEOVER:
            std::cerr << "Got MSG_GAMEOVER\n";
            gameOverMessage(payload);
            break;
        case MSG_NEWROUND:
            std::cerr << "Got MSG_NEWROUND\n";
            newRoundMessage(payload);
            break;
        case MSG_ENDROUND:
            std::cerr << "Got MSG_ENDROUND\n";
            endRoundMessage(payload);
            break;
        case MSG_ATTACKERPASSED:
            std::cerr << "Got MSG_ATTACKERPASSED\n";
            attackerPassedMessage(payload);
            break;
        case MSG_ATTACKINGCARD:
            std::cerr << "Got MSG_ATTACKINGCARD\n";
            attackingCardMessage(payload);
            break;
        case MSG_DEFENDINGCARD:
            std::cerr << "Got MSG_DEFENDINGCARD\n";
            defendingCardMessage(payload);
            break;
        case MSG_PILEDONCARD:
            std::cerr << "Got MSG_PILEDONCARD\n";
            piledOnCardMessage(payload);
            break;
        case MSG_PLAYEDOUT:
            std::cerr << "Got MSG_PLAYEDOUT\n";
            playedOutMessage(payload);
            break;
        case MSG_GIVENCARDSN:
            std::cerr << "Got MSG_GIVENCARDSN\n";
            givenCardsNMessage(payload);
            break;
        case MSG_GIVENCARDSCS:
            std::cerr << "Got MSG_GIVENCARDSCS\n";
            givenCardsCSMessage(payload);
            break;
        case MSG_ATTACK:
            std::cerr << "Got MSG_ATTACK\n";
            attackMessage(payload);
            break;
        case MSG_DEFEND:
            std::cerr << "Got MSG_DEFEND\n";
            defendMessage(payload);
            break;
        case MSG_PILEON:
            std::cerr << "Got MSG_PILEON\n";
            pileOnMessage(payload);
            break;
        case MSG_PLAYED:
            std::cerr << "Got MSG_PLAYED\n";
            std::cerr << "Should not have gotten this message!\n";
            break;
        case MSG_ADDCARDS:
            std::cerr << "Got MSG_ADDCARDS\n";
            addCardsMessage(payload);
            break;
        default:
            std::cerr << "NetworkGame: Unknown message type\n";
        }
    }

}

void NetworkGame::endMessage(const std::string &payload)
{
    network_running = false;
    running_ = false;
}

void NetworkGame::gameStartingMessage(const std::string &payload)
{
    // Read trump, player name array
    trumpCard_ = readCard(payload);
    // Build players array, and give to super class
    int numPlayers = payload[2];
    assert(numPlayers >= 2 && numPlayers <= 6);
    int stridx = 3;
    for (int i = 0; i < numPlayers; i++)
    {
        string name = readString(string(payload.c_str()+stridx));
        stridx += name.length() + 1;

        PlayerPtr p;
        // Are they a localPlayer?
        if (name.find('#') != string::npos)
        {
            assert(localPlayer_.get());
            p = localPlayer_;
        }
        // if not, use a proxy
        else
        {
            p = PlayerPtr(new ProxyPlayer(name));
        }
        players_.push_back(p);
    }
    // Set deckSize
    deckSize_ = 36 - numPlayers * Game::HAND_SIZE;
    discardSize_ = 0;
    // Tell the player, if they exist
    if (localPlayer_.get())
        localPlayer_->gameStarting(this);
    // Broadcast
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->gameStart(this);
}

void NetworkGame::gameOverMessage(const std::string &payload)
{
    assert(payload.size() == 1);
    ConstPlayerPtr biscuit = readPlayer(payload, players_);
    // Broadcast
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->gameOver(biscuit);
}

void NetworkGame::newRoundMessage(const std::string &payload)
{
    assert(payload.size() == 2);
    // Set attacker
    attacker_ = readPlayer(payload, players_);
    // Set defender
    string s = ""; s.push_back(payload[1]);
    defender_ = readPlayer(s, players_);
    // Reset variables
    playedCards_.clear();
    playableRanks_.clear();
    tricksLeft_ = std::min(defender_->getNumCards(), Game::HAND_SIZE);
    // Broadcast
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->newRound(attacker_, defender_);
}

void NetworkGame::endRoundMessage(const std::string &payload)
{
    assert(payload.size() == 1);
    bool successfulDefend = readBool(payload);
    // On successful defend, add to discard size
    if (successfulDefend)
        discardSize_ += playedCards_.size();
    // Broadcast
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->endRound(successfulDefend);
}

void NetworkGame::attackerPassedMessage(const std::string &payload)
{
    assert(payload.size() == 1);
    // Set new attacker
    attacker_ = readPlayer(payload, players_);
    // Broadcast
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->attackerPassed(attacker_);
}

void NetworkGame::attackingCardMessage(const std::string &payload)
{
    assert(payload.size() == 2);
    Card c = readCard(payload);
    // Remove card from attacker_, but only if it's a proxy, otherwise
    // it has already been removed
    if (attacker_ != localPlayer_)
    {
        ProxyPlayer *pp = (ProxyPlayer *) attacker_.get();
        pp->removeCards(1);
    }
    // Add to played cards & playableRanks
    playedCards_.push_back(c);
    playableRanks_.insert(c);
    // update tricksLeft
    tricksLeft_--;
    // Broadcast
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->attackingCard(c);
}

void NetworkGame::defendingCardMessage(const std::string &payload)
{
    assert(payload.size() == 2);
    // Broadcast
    std::cerr << "Got MSG_PILEDONCARD\n";
    assert(payload.size() == 2);
    Card c = readCard(payload);
    // Remove card from defender_, but only if it's a proxy, otherwise
    // it has already been removed
    if (defender_ != localPlayer_)
    {
        ProxyPlayer *pp = (ProxyPlayer *) defender_.get();
        pp->removeCards(1);
    }
    // Add to played cards & playableRanks
    playedCards_.push_back(c);
    playableRanks_.insert(c);
    // Broadcast
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->defendingCard(c);
}

void NetworkGame::piledOnCardMessage(const std::string &payload)
{
    assert(payload.size() == 2);
    Card c = readCard(payload);
    // Remove card from attacker_, but only if it's a proxy, otherwise
    // it has already been removed
    if (attacker_ != localPlayer_)
    {
        ProxyPlayer *pp = (ProxyPlayer *) attacker_.get();
        pp->removeCards(1);
    }
    // Add to played cards & playableRanks
    playedCards_.push_back(c);
    playableRanks_.insert(c);
    // update tricksLeft
    tricksLeft_--;
    // Broadcast
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->piledOnCard(c);
}

void NetworkGame::playedOutMessage(const std::string &payload)
{
    assert (payload.size() == 1);
    ConstPlayerPtr p = readPlayer(payload, players_);
    // Broadcast
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->playedOut(p);
}

void NetworkGame::givenCardsNMessage(const std::string &payload)
{
    assert(payload.size() == 2);
    PlayerPtr p = readPlayer(payload, players_);
    int n = payload[1];
    // Remove cards n from the deck
    assert(deckSize_ >= n);
    deckSize_ -= n;
    // Only if they're a proxy
    if (p != localPlayer_)
    {
        // Add n cards to player
        std::vector<Card> cs(n);
        p->addCards(cs);
    }
    // Broadcast
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->givenCards(p, n);
}

void NetworkGame::givenCardsCSMessage(const std::string &payload)
{
    // Add cs.size cards to player
    PlayerPtr p = readPlayer(payload, players_);
    std::vector<Card> cs = readCards(string(payload.c_str()+1, payload.size()-1));
    // Only if they're a proxy
    if (p != localPlayer_)
    {
        p->addCards(cs);
    }
    // Broadcast
    for (lit_ = listeners_.begin(); lit_ != listeners_.end(); lit_++)
        (*lit_)->givenCards(p, cs);
}

void NetworkGame::attackMessage(const std::string &payload)
{
    assert(payload.size() == 0);
    if (localPlayer_.get())
    {
        assert(localPlayer_->getNumCards() > 0);
        // Get the card from the player and send it across the wire
        Card c = localPlayer_->attack(playableRanks_);
        sock_->send(createMessage(MSG_PLAYED, serializeCard(c)));
    }
    else
        std::cerr << "ERROR: got attack message with no local player\n";
}

void NetworkGame::defendMessage(const std::string &payload)
{
    assert(payload.size() == 0);
    if (localPlayer_.get())
    {
        assert(localPlayer_->getNumCards() > 0);
        // Get the card from the player and send it across the wire
        Card c = localPlayer_->defend(playedCards_.back(), trumpCard_.getSuit());
        sock_->send(createMessage(MSG_PLAYED, serializeCard(c)));
    }
    else
        std::cerr << "ERROR: got defend message with no local player\n";
}

void NetworkGame::pileOnMessage(const std::string &payload)
{
    assert(payload.size() == 0);
    if (localPlayer_.get())
    {
        assert(localPlayer_->getNumCards() > 0);
        // Get the card from the player and send it across the wire
        Card c = localPlayer_->pileOn(playableRanks_);
        sock_->send(createMessage(MSG_PLAYED, serializeCard(c)));
    }
    else
        std::cerr << "ERROR: got pileOn message with no local player\n";
}

void NetworkGame::addCardsMessage(const std::string &payload)
{
    if (localPlayer_.get())
    {
        std::vector<Card> cards = readCards(payload);
        localPlayer_->addCards(cards);
    }
    else
        std::cerr << "ERROR: got addCards message with no local player\n";
}

bool NetworkGame::connectTo(kissnet::tcp_socket_ptr sock)
{
    sock_ = sock;
    connected_ = true;

    std::cout << "Used passed socket as connection\n";

    return connected_;
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

