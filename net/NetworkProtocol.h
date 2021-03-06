#pragma once
#include <string>
#include <vector>
#include "core/Card.h"
#include "core/Player.h"

enum network_message_t
{
    // Network Management messages
    MSG_READY = 1,         // For the start of a session
    MSG_NAME =  2,
    MSG_END =   3,               // For the end of a session
    // Game listener messages
    MSG_GAMESTARTING   = 10,
    MSG_GAMEOVER       = 11,
    MSG_NEWROUND       = 12,
    MSG_ATTACKERPASSED = 13,
    MSG_ENDROUND       = 14,
    MSG_ATTACKINGCARD  = 15,
    MSG_DEFENDINGCARD  = 16,
    MSG_PILEDONCARD    = 17,
    MSG_PLAYEDOUT      = 18,
    MSG_GIVENCARDSN    = 19,
    MSG_GIVENCARDSCS   = 20,
    // Player Messages
    MSG_ATTACK   = 100,
    MSG_DEFEND   = 101,
    MSG_PILEON   = 102,
    MSG_PLAYED   = 103,
    MSG_ADDCARDS = 104,
};

/** 
 * Creates a message ready for sending given a type and payload.
 * Message format is documented in the EZDurak trac.
 * 
 * @param type The message type
 * @param payload the payload
 * 
 * @return A message ready for sending.
 */
std::string createMessage(char type, const std::string &payload);

/** 
 * Creates a serialized card object
 * 
 * @param c The card to serialize
 * 
 * @return The serialized card
 */
std::string serializeCard(const Card &c);
/** 
 * Serializes a list of cards according to the serialization rules.
 * 
 * @param cs The list of cards to serialize
 * 
 * @return  The serialized cards
 */
std::string serializeCards(const std::vector<Card> &cs);
/** 
 * Serializes a player as an index
 * 
 * @param p The player to serialize
 * @param ps The players array (to find the index)
 * 
 * @return The serialized player
 */
std::string serializePlayer(ConstPlayerPtr p, const std::vector<PlayerPtr>& ps);
/** 
 * Simply null terminates the string.
 * 
 * @param s String to serialize
 * 
 * @return  Serialized string
 */
std::string serializeString(const std::string &s);
/** 
 * Serializes a boolean value
 * 
 * @param b The boolean to serialize
 * 
 * @return The serialized boolean
 */
std::string serializeBool(bool b);

/** 
 * Unserializes a card.
 * 
 * @param in The serialized card
 * 
 * @return The card
 */
Card              readCard(const std::string& in);
/** 
 * Deserializes a list of cards
 * 
 * @param in Serialized list of cards
 * 
 * @return  The cards
 */
std::vector<Card> readCards(const std::string& in);
/** 
 * Deserializes a player
 * 
 * @param in The serialized player
 * @param ps The player list
 * 
 * @return The player
 */
PlayerPtr         readPlayer(const std::string& in, const std::vector<PlayerPtr>& ps);
/** 
 * Deserializes a string
 * 
 * @param in The serialized string
 * 
 * @return  The string
 */
std::string       readString(const std::string& in);
/** 
 * Deserializes a boolean
 * 
 * @param in The serialized boolean
 * 
 * @return The boolean
 */
bool              readBool(const std::string& in);

