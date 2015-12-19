//
// Created by hubert on 05.12.15.
//

#ifndef RING_MESSAGETYPES_H
#define RING_MESSAGETYPES_H

enum MessageType{
    UNDEFINED = 0,
    GET = 1,

    CREATE_CATEGORY = 2,
    DESTROY_CATEGORY = 3,
    CATEGORY_LIST = 4,
    JOIN_CATEGORY = 5,
    LEFT_CATEGORY = 6,
    SIGN_UP_CATEGORY = 7,
    SIGN_OUT_CATEGORY = 8,
    RING_MESSAGE = 9,
    NEIGHBOURS_SET = 10,
    SERVER_INFO = 11
};


enum GetMessageType{
    CAT_LIST = 1,
    NEIGHBOURS = 2,
};

enum ServerInfoMessageType{
    OK = 0,
    FAIL = 1,
    PERMISSION_DENIED = 2,
    USER_ADDED = 3,
    CATEGORY_CREATED = 4,
    CATEGORY_REMOVED = 5,
};
#endif //RING_MESSAGETYPE_H
