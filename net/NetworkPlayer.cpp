#include "NetworkPlayer.h"

NetworkPlayer::NetworkPlayer(GameAgent *agent) :
    NetworkListener(agent),
    name_("Uninitialized NetworkPlayer")
{
    // TODO Initialize hand_ member, see NetworkListener.h
    // (You may not need to)
}

NetworkPlayer::~NetworkPlayer()
{
    // Taken care of by ~NetworkListener
}

bool NetworkPlayer::getConnection(const std::string &port)
{
    try
    {
        tcp_socket_ptr servsock = tcp_socket::create();
        servsock->listen(port, 5);
        clisock_ = servsock->accept();

        // Check for handshake messages
        bool ready = false;
        while (!ready)
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
                name_ = string(clisock_, payload_size);
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
    catch (socket_exception &e)
    {
        std::cerr << "Unable to getConnection: " << e.what() << '\n';
        connected_ = false;
    }

    return connected_;
}

void NetworkPlayer::gameStarting(GameAgent *agent)
{
    // read MSG_NAME and set name
}

Card NetworkPlayer::defend(const Card &attc, Card::cardsuit trump)
{
    // Send MSG_DEFEND

    // Wait for MSG_PLAYED
    // Use a 3 byte character array as a header.
    char header[3];
    // Then read in the header from the socket, the socket is from NetworkListener
    int bytes_recieved = clisock_->recv(header, 3); // buffer, length
    // Make sure that bytes_recieved is 3
    // Make sure that size is 2 (the size of a serialized card)
    int size = header[0] + header[1] * 256;
    // Make sure that type is MSG_PLAYED
    char type = header[2];
    // Read the rest of the data
    char payload[2];
    bytes_recieved = clisock_->recv(payload, 2);
    // Check bytes_received = 2

    // Make sure the card is valid, if not, start from the top

    // Remove the card from the hand, return it
}

Card NetworkPlayer::attack(std::set<int> playableRanks);
{
    // Send MSG_ATTACK

    // Wait for MSG_PLAYED

    // Make sure the card is valid, if not, start from the top

    // Remove the card from the hand, return it
}

Card NetworkPlayer::pileOn(std::set<int> playableRanks);
{
    // Send MSG_PILEON

    // Wait for MSG_PLAYED

    // Make sure the card is valid, if not, start from the top

    // Remove the card from the hand, return it
}

void NetworkPlayer::addCards(const std::vector<Card> &cards)
{
    // Add Cards

    // Send over network
}

int NetworkPlayer::getNumCards() const
{
}

std::string NetworkPlayer::getName() const
{
}
