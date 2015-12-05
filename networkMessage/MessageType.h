//
// Created by hubert on 05.12.15.
//

#ifndef RING_MESSAGETYPE_H
#define RING_MESSAGETYPE_H

enum MessageType{
    UNDEFINED = 0,
    GET = 1,

    CREATE_CATEGORY = 2,
    DESTROY_CATEGORY = 3,
    CATEGORY_LIST = 4,
    JOIN_CATEGORY = 5,
    LEFT_CATEGORY = 6,
    RING_MESSAGE = 7
};

#endif //RING_MESSAGETYPE_H
