#include "NetworkProtocol.h"
#include <iostream>

int main()
{
    // Card
    Card c1(10, Card::hearts);
    assert(c1 == readCard(serializeCard(c1)));
    Card c;
    assert(c == readCard(serializeCard(c)));

    // Cards
    std::vector<Card> cs1;
    cs1.push_back(c1);
    cs1.push_back(c);
    cs1.push_back(Card(Card::ACE, Card::clubs));

    std::string serialized = serializeCards(cs1);
    std::cout.write(serialized.c_str(), serialized.size());

    std::vector<Card> res = readCards(serializeCards(cs1));
    assert(res.size() == cs1.size());
    for (int i = 0; i < res.size(); i++)
        assert(res[i] == cs1[i]);

    // Bool
    assert(true == readBool(serializeBool(true)));
    assert(false == readBool(serializeBool(false)));

    // String
    assert("" == readString(serializeString("")));
    assert("hello fred" == readString(serializeString("hello fred")));

    // TODO Players
}
