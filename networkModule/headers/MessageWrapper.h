//
// Created by hubert on 28.12.15.
//

#ifndef RING_MESSAGEWRAPPER_H
#define RING_MESSAGEWRAPPER_H

#include <string>
#include <memory>
#include "../../networkMessage/headers/SimpleMessage.h"


class MessageWrapper {
private:
    std::shared_ptr<SimpleMessage> message;
    std::string ip;
    int port;
public:
    MessageWrapper(std::shared_ptr<SimpleMessage> message, std::string ip, int port);

    std::shared_ptr<SimpleMessage> getMessage() const;

    std::string getIP() const;

    int getPort() const;
};


#endif //RING_MESSAGEWRAPPER_H
