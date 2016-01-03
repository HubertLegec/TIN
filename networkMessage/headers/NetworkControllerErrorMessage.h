//
// Created by hubert on 03.01.16.
//

#ifndef RING_NETWORKCONTROLLERERRORMESSAGE_H
#define RING_NETWORKCONTROLLERERRORMESSAGE_H

#include "SimpleMessage.h"

class NetworkControllerErrorMessage : public SimpleMessage {

public:
    enum ErrorCode {
        UNDEFINED = 0,
        CANT_CONNECT_TO_SERVER = 1
        //TODO
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
