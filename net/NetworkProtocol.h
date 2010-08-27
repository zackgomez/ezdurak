#pragma once
#include <string>
#include <vector>
#include "core/Card.h"
#include "core/Player.h"

enum network_message_t
{
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
    MSG_GIVENCARDSCS
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
