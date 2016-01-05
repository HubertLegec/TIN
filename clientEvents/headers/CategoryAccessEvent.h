#ifndef RING_CATEGORYACCESSEVENT_H
#define RING_CATEGORYACCESSEVENT_H

#include "BasicEvent.h"

/**
 * This class is used to transfer all events connected with category management
 * Setting proper access type, we can create, delete, sign up, sign out, join and leave category
 */
class CategoryAccessEvent : public BasicEvent {

public:
    enum AccessType {
        CREATE_CATEGORY,
        DELETE_CATEGORY,
        SIGN_UP_CATEGORY,
        JOIN_CATEGORY,
        SIGN_OUT_CATEGORY,
        LEAVE_CATEGORY
    };

    CategoryAccessEvent(AccessType type, const std::string &categoryName);

    CategoryAccessEvent(AccessType type, long categoryID);

    AccessType getType();

    const std::string &getCategoryName() const

    long getCategoryID() const;

    virtual std::string toString() const;

    virtual std::string getName() const;

private:
    AccessType type;
    std::string categoryName;
    long categoryId;
};

#endif //RING_CATEGORYACCESSEVENT_H