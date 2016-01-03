//
// Created by hubert on 05.12.15.
//

#ifndef RING_MESSAGETYPES_H
#define RING_MESSAGETYPES_H

enum MessageType{
    UNDEFINED = 0,
    //GetMessage
    GET = 1,
    //CategoryListMessage
            CATEGORY_LIST = 2,
    //CategoryManagementMessage
            CREATE_CATEGORY = 3,
    DESTROY_CATEGORY = 4,
    JOIN_CATEGORY = 5,
    LEFT_CATEGORY = 6,
    ACTIVATE_CATEGORY = 7,
    DEACTIVATE_CATEGORY = 8,
    //RingMessage
    RING_MESSAGE = 9,
    //NeighboursInfoMessage
    NEIGHBOURS_SET = 10,
    //ServerInfoMessage
    SERVER_INFO = 11,
    //UserManagementMessage
    CREATE_USER_ACCOUNT = 12,
    DELETE_USER_ACCOUNT = 13,
    CLIENT_CLOSE_APP = 14,
    //this type isn't sent via the Net, NetworkController put it to receive queue, to inform about problems
    NETWORK_CONTROLLER_ERROR_MESSAGE = 15
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
    CATEGORY_JOINED = 6,
    CATEGORY_LEFT = 7,
    CATEGORY_ACTIVATED = 8,
    CATEGORY_DEACTIVATED = 9,
    USER_CREATED = 10,
    USER_DELETED = 11
};
#endif //RING_MESSAGETYPE_H
