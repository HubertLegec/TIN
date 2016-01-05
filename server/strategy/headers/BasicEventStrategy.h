#ifndef RING_BASICEVENTSTRATEGY_H
#define RING_BASICEVENTSTRATEGY_H

#include "../../../networkMessage/headers/SimpleMessage.h"
#include <string>

using namespace std;

class Controller;
class CategoryMember;

class BasicEventStrategy {
protected:
    Controller *controller;

    void sendMessage(long userID, ServerInfoMessageType infoMessageType) const;

    void sendMessage(long userID, ServerInfoMessageType infoMessageType, const string& info) const;

    void sendMessage(long userID, long extraInfo, ServerInfoMessageType infoMessageType) const;

    void sendMessage(long userID, long extraInfo, ServerInfoMessageType infoMessageType, const string &info) const;

    void sendNeighbours(long categoryID, shared_ptr<CategoryMember> member) const;

    void sendNeighbours(long categoryID, long memberID) const;

    void sendAllNeighbours(long categoryID, long memberID) const;

    void sendForAllMembers(long categoryID, SimpleMessage *message) const;

public:
    BasicEventStrategy();

    BasicEventStrategy(Controller *controller);

    void setController(Controller *controller);

    virtual void serveEvent(SimpleMessage *message) const = 0;
};


#endif //RING_BASICEVENTSTRATEGY_H
