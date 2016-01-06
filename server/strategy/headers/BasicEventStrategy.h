#ifndef RING_BASICEVENTSTRATEGY_H
#define RING_BASICEVENTSTRATEGY_H

#include "../../../networkMessage/headers/SimpleMessage.h"
#include <string>

using namespace std;

class Category;

class Controller;

class CategoryMember;

class User;

class BasicEventStrategy {
protected:
    Controller *controller;

    void sendMessage(shared_ptr<User> user, ServerInfoMessageType infoMessageType) const;

    void sendMessage(shared_ptr<User> user, ServerInfoMessageType infoMessageType, const string &info) const;

    void sendMessage(shared_ptr<User> user, long extraInfo, ServerInfoMessageType infoMessageType) const;

    void sendMessage(shared_ptr<User> user, long extraInfo, ServerInfoMessageType infoMessageType,
                     const string &info) const;

    void sendNeighbours(shared_ptr<Category> categoryID, shared_ptr<CategoryMember> member) const;

    void sendAllNeighbours(shared_ptr<Category> category, shared_ptr<CategoryMember> member) const;

    void sendForAllMembers(shared_ptr<Category> category, shared_ptr<SimpleMessage> message) const;

    void sendForAllMembers(shared_ptr<Category> category, long extraInfo, ServerInfoMessageType infoMessageType) const;

    void sendCategoryNotFound(shared_ptr<User> sender, long categoryID) const;

    void sendUserNotFound(shared_ptr<User> sender, long userID) const;

    void sendMemberNotFound(shared_ptr<User> sender, long categoryID, long userID) const;

public:
    BasicEventStrategy();

    BasicEventStrategy(Controller *controller);

    void setController(Controller *controller);

    virtual void serveEvent(SimpleMessage *message) const = 0;
};


#endif //RING_BASICEVENTSTRATEGY_H
