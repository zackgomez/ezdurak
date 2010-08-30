#include "NetworkPlayer.h"
#include "NetworkProtocol.h"
#include "core/GameAgent.h"
#include <cassert>
#include <iostream>
#include <string>

using std::string;

NetworkPlayer::NetworkPlayer() :
    NetworkListener(),
    name_("Uninitialized NetworkPlayer")
{
}

NetworkPlayer::~NetworkPlayer()
{
    // Taken care of by ~NetworkListener
}

bool NetworkPlayer::getConnection(const std::string &port)
{
    try
    {
        kissnet::tcp_socket_ptr servsock = kissnet::tcp_socket::create();
        servsock->listen(port, 5);
        clisock_ = servsock->accept();

        // Check for handshake messages
        connected_ = false;
        while (!connected_)
        {
            char header[3];
            clisock_->recv(header, 3);
            if (header[2] == MSG_READY)
            {
                connected_ = true;
                std::cout << "Got connection and ready message!\n";
            }
            else if (header[2] == MSG_NAME)
            {
                int payload_size = header[0] + 256 * header[1];
                char *payload = new char[payload_size];
                clisock_->recv(payload, payload_size);
                name_ = std::string(payload, payload_size - 1);
                delete payload;
            }
            else
            {
                std::cout << "Got unknown message during handshake\n";
                std::cout << "Header: "; std::cout.write(header,3);
                std::cout << '\n';
            }
        }
    }
    catch (kissnet::socket_exception &e)
    {
        std::cerr << "Unable to getConnection: " << e.what() << '\n';
        connected_ = false;
    }

    return connected_;
}

void NetworkPlayer::gameStarting(GameAgent *agent)
{
    agent->addListener(this);
}

Card NetworkPlayer::defend(const Card &attc, Card::cardsuit trump)
{
    Card defCard;
    for (;;)
    {
        // Send MSG_DEFEND
        string message = createMessage(MSG_DEFEND, "");
        clisock_->send(message);


        // Wait for MSG_PLAYED
        // Use a 3 byte character array as a header.
        char header[3];
        // Then read in the header from the socket, the socket is from NetworkListener
        int bytes_received = clisock_->recv(header, 3); // buffer, length
        // Make sure that bytes_recieved is 3
        assert (bytes_received == 3);
        // Make sure that size is 2 (the size of a serialized card)
        int size = header[0] + header[1] * 256;
        assert(size == 2);
        // Make sure that type is MSG_PLAYED
        char type = header[2];
        assert(type == MSG_PLAYED);
        // Read the rest of the data
        char payload[2];
        bytes_received = clisock_->recv(payload, 2);
        // Check bytes_received = 2
        assert(bytes_received == 2);

        // Make sure the card is valid, if not, start from the top
        defCard = readCard(payload);
        if (defCard.beats(attc, trump))
        {
            assert (hand_.find(defCard) != hand_.end());
            // Remove the card from the hand, return it
            hand_.erase(hand_.find(defCard));
            return defCard;
        }
        if (!defCard)
        {
            return defCard;
        }
    }

}

Card NetworkPlayer::attack(std::set<int> playableRanks)
{
    Card attCard;
    for (;;)
    {
        // Send MSG_ATTACK
        string message = createMessage(MSG_ATTACK, "");
        clisock_->send(message);

        // Wait for MSG_PLAYED
        // Use a 3 byte character array as a header.
        char header[3];
        // Then read in the header from the socket, the socket is from NetworkListener
        int bytes_received = clisock_->recv(header, 3); // buffer, length
        // Make sure that bytes received = 3
        assert (bytes_received == 3);
        //Make sure that size is 2 (the size of a serialized card)
        int size = header[0] + header[1] * 256;
        assert(size == 2);
        // Make sure that type is MSG_PLAYED
        char type = header[2];
        assert(type == MSG_PLAYED);
        // Read the rest of the data
        char payload[2];
        bytes_received = clisock_->recv(payload, 2);
        // Check bytes_received = 2
        assert(bytes_received == 2);

        // Make sure the card is valid, if not, start from the top
        attCard = readCard(payload);
        if (attCard && (playableRanks.find(attCard) != playableRanks.end() 
                        || playableRanks.size() != 0))
        {
            assert (hand_.find(attCard) != hand_.end());
            // Remove the card from the hand, return it
            hand_.erase(hand_.find(attCard));
            return attCard;
        }
        if (!attCard && playableRanks.size() != 0)
        {
            return attCard;
        }

    }
}

Card NetworkPlayer::pileOn(std::set<int> playableRanks)
{
    Card pCard;
    for (;;)
    {
        // Send MSG_PILEON
        string message = createMessage(MSG_PILEON, "");
        clisock_->send(message);

        // Wait for MSG_PLAYED
        // Use a 3 byte character array as a header.
        char header[3];
        // Then read in the header from the socket, the socket is from NetworkListener
        int bytes_received = clisock_->recv(header, 3); // buffer, length
        // Make sure that bytes received = 3
        assert (bytes_received == 3);
        //Make sure that size is 2 (the size of a serialized card)
        int size = header[0] + header[1] * 256;
        assert(size == 2);
        // Make sure that type is MSG_PLAYED
        char type = header[2];
        assert(type == MSG_PLAYED);
        // Read the rest of the data
        char payload[2];
        bytes_received = clisock_->recv(payload, 2);
        // Check bytes_received = 2
        assert(bytes_received == 2);

        // Make sure the card is valid, if not, start from the top
        pCard = readCard(payload);
        if (playableRanks.find(pCard) != playableRanks.end())
        {
            assert(hand_.find(pCard) != hand_.end());
            // Remove the card from the hand, return it
            hand_.erase(hand_.find(pCard));
            return pCard;
        }
        if (!pCard)
        {
            return pCard;
        }

    }
}

void NetworkPlayer::addCards(const std::vector<Card> &cards)
{
    // Add Cards
    for (int i = 0; i < cards.size(); i++)
    {
        hand_.insert(cards[i]);
    }

    // Send over network
    string payload = serializeCards(cards);
    string message = createMessage(MSG_ADDCARDS, payload);
    clisock_->send(message);
}

int NetworkPlayer::getNumCards() const
{
    return hand_.size();
}

std::string NetworkPlayer::getName() const
{
    return name_;
}
