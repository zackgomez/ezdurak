#pragma once
#include <string>
#include <vector>
#include "core/Card.h"
#include "core/Player.h"

enum network_message_t
{
    // Network Management messages
    MSG_READY = 1,         // For the start of a session
    MSG_NAME,
    MSG_END,               // For the end of a session
    // Game listener messages
    MSG_GAMESTARTING = 10,
    MSG_GAMEOVER,
    MSG_NEWROUND,
    MSG_ATTACKERPASSED,
    MSG_ENDROUND,
    MSG_ATTACKINGCARD,
    MSG_DEFENDINGCARD,
    MSG_PILEDONCARD,
    MSG_PLAYEDOUT,
    MSG_GIVENCARDSN,
    MSG_GIVENCARDSCS,
    // Player Messages
    MSG_ATTACK = 100,
    MSG_DEFEND,
    MSG_PILEON,
    MSG_PLAYED,
    MSG_ADDCARDS,
};

std::string createMessage(char type, const std::string &payload);

std::string serializeCard(const Card &c);
std::string serializeCards(const std::vector<Card> &cs);
std::string serializePlayer(ConstPlayerPtr p, const std::vector<PlayerPtr>& ps);
std::string serializeString(const std::string &s);
std::string serializeBool(bool b);

Card              readCard(const std::string& in);
std::vector<Card> readCards(const std::string& in);
PlayerPtr         readPlayer(const std::string& in, const std::vector<PlayerPtr>& ps);
std::string       readString(const std::string& in);
bool              readBool(const std::string& in);
