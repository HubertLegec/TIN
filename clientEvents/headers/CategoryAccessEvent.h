#ifndef RING_CATEGORYACCESSEVENT_H
#define RING_CATEGORYACCESSEVENT_H

#include "BasicEvent.h"

class CategoryAccessEvent : public BasicEvent {

public:
    enum AccessType {
        CREATE_CATEGORY,
        DELETE_CATEGORY,
        REGISTER_IN_CATEGORY,
        JOIN_CATEGORY,
        VISIT_CATEGORY,
        LEAVE_CATEGORY
    };

    CategoryAccessEvent(AccessType type, const std::string & categoryName, const std::string & userName = "",const std::string & userPassword = "");

    AccessType getType();
    std::string getCategoryName();
    std::string getUserName();
    std::string getPassword();

    std::string toString();

private:
    AccessType type;

    std::string categoryName;
    std::string userName;
    std::string userPassword;
};


#endif //RING_CATEGORYACCESSEVENT_H