//
// Created by hubert on 03.01.16.
//

#ifndef RING_NETWORKCONTROLLERERRORMESSAGE_H
#define RING_NETWORKCONTROLLERERRORMESSAGE_H

#include "SimpleMessage.h"

/**
 * This class isn't serialized and send via sockets
 * Network controller uses it to inform client or server about some troubles
 */
class NetworkControllerErrorMessage : public SimpleMessage {

public:
    enum ErrorCode {
        UNDEFINED = 0,
        UNABLE_TO_CREATE_LISTENING_SOCKET = 1,
        UNABLE_TO_SEND_MSG = 2,
        UNABLE_TO_RESERIALIZE_MSG = 3,
        UNABLE_TO_SERIALIZE_MSG = 4
    };

    NetworkControllerErrorMessage();

    NetworkControllerErrorMessage(ErrorCode code, const std::string &info);

    ErrorCode getErrorCode() const;

    void setErrorCode(ErrorCode code);

    const std::string &getInfo() const;

    void setInfo(const std::string &info);

    virtual std::string toString() const;

private:
    ErrorCode code;
    std::string info;
};


#endif //RING_NETWORKCONTROLLERERRORMESSAGE_H
