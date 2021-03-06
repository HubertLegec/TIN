//
// Created by hubert on 05.12.15.
//

#ifndef RING_SERVERINFOMESSAGE_H
#define RING_SERVERINFOMESSAGE_H

#include <string>
#include "SimpleMessage.h"
#include "../../cereal/archives/binary.hpp"
#include "../../cereal/types/string.hpp"

/**
 * This class is used by server to send information about success or fail of the operation
 * It also transfer to client (category owner) information about user request of joining to some category
 */
class ServerInfoMessage : public SimpleMessage {
private:
    ServerInfoMessageType infoType;
    /* additional field - can be used for example for passing added user id */
    long extraInfo;
    std::string info;
public:
    ServerInfoMessage();

    ServerInfoMessage(long senderID, ServerInfoMessageType infoType, const std::string &info);

    ServerInfoMessageType getInfoType() const;

    void setExtraInfo(long extraInfo);

    long getExtraInfo() const;

    const std::string &getInfo() const;

    void setInfo(const std::string &info);

    void setServerInfoMessageType(ServerInfoMessageType infoType);

    virtual std::string toString() const;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(this->type, this->size, this->senderID, this->infoType, this->extraInfo, this->info);
    }
};


#endif //RING_SERVERINFOMESSAGE_H
