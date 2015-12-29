//
// Created by hubert on 06.12.15.
//

#ifndef RING_RINGMESSAGE_H
#define RING_RINGMESSAGE_H

#include <string>
#include <vector>
#include "SimpleMessage.h"
#include "../../cereal/archives/binary.hpp"
#include "../../cereal/types/string.hpp"
#include "../../cereal/types/vector.hpp"


class RingMessage : public SimpleMessage {
private:
    long categoryId;
    std::string messageText;
    std::vector<std::string> confirmations;
public:
    RingMessage();
    RingMessage(long senderID, long categoryId, const std::string& text);
    RingMessage(long senderID, long categoryId, const std::string& text, const std::vector<std::string>& confirmations);
    RingMessage(const RingMessage& other);
    RingMessage& operator=(const RingMessage& other);
    std::vector<std::string> getConfirmationsList() const;
    void addConfirmation(const std::string& name);
    void addConfirmations(const std::vector<std::string>& nameList);

    const std::string &getMsgText() const;
    long getCategoryId() const;

    virtual std::string toString() const;
    template<class Archive>
    void serialize(Archive & archive){
        archive(this->type, this->size, this->senderID, this->categoryId, this->messageText, this->confirmations);
    }
};


#endif //RING_RINGMESSAGE_H
