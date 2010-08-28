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
