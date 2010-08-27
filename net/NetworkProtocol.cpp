#include "NetworkProtocol.h"
#include <string>
#include <sstream>

using std::string;
using std::vector;
using std::stringstream;

string createMessage(char type, const string& payload)
{
    stringstream ss;

    // Size must fit in 2 bytes
    assert (payload.size() < 256*256);
    short size = payload.size();

    // First write the 2 byte size
    ss.put(size % 256);
    ss.put(size / 256);

    // Then the 1 byte type
    ss.write(&type, 1);

    // And finally the payload
    ss.write(payload.c_str(), payload.size());

    return ss.str();
}

string serializeCard(const Card &c)
{
    char ret[2];
    ret[0] = (char) c.getSuit();
    ret[1] = (char) c.getNum();

    return std::string(ret, 2);
}

string serializeCards(const vector<Card> &cs)
{
    // Must fit in 1 byte
    assert(cs.size() < 256);

    stringstream ss;

    // First write the 1 byte n
    char size = cs.size();
    ss.write(&size, 1);

    // Write each 2 byte card
    for (int i = 0; i < size; i++)
        ss.write(serializeCard(cs[i]).c_str(), 2);

    return ss.str();
}

string serializePlayer(ConstPlayerPtr p, const vector<PlayerPtr> &ps)
{
    char i;
    for (i = 0; i < ps.size(); i++)
        if (ps[i] == p)
            break;
    assert (i != ps.size() && "Unable to find player");

    return string(&i, 1);
}

string serializeString(const std::string &s)
{
    // Just need to null terminate...
    string ret(s);
    ret.push_back('\0');
    return ret;
}

string serializeBool(bool b)
{
    return b ? string("1", 1) : string("0", 1);
}

Card readCard(const string &s)
{
    assert(s.size() >= 2);
    
    Card::cardsuit suit = (Card::cardsuit) s[0];
    int rank = s[1];

    // TODO Some kind of error checking here
    
    if (suit == Card::none)
        return Card();

    return Card(rank, suit);
}

vector<Card> readCards(const string &s)
{
    char size = s[0];
    int i = 1;
    vector<Card> ret;

    while ((i - 1) / 2 < size)
    {
        string cur(&s.c_str()[i], 2);
        ret.push_back(readCard(cur));
        i += 2;
    }

    return ret;
}

PlayerPtr readPlayer(const string &s, const vector<PlayerPtr> &ps)
{
    int i = s[0];
    assert(i < ps.size());

    return ps[i];
}

string readString(const string &s)
{
    return string(s.c_str());
}

bool readBool(const string &s)
{
    assert (!s.empty());

    return s[0] != '0';
}
