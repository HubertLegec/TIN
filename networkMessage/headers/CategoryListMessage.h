//
// Created by hubert on 05.12.15.
//

#ifndef RING_CATEGORYLISTMESSAGE_H
#define RING_CATEGORYLISTMESSAGE_H

#include <vector>
#include <string>
#include "SimpleMessage.h"
#include "../../cereal/archives/binary.hpp"
#include "../../cereal/types/string.hpp"
#include "../../cereal/types/map.hpp"

class CategoryListMessage : public SimpleMessage {
private:
    std::map<long, std::string> categories;

public:
    CategoryListMessage();
    CategoryListMessage(long senderID, const std::map<long, std::string>& categories);
    void addCategory(long categoryID, std::string categoryName);
    std::map<long, std::string> getCategories() const;

    virtual std::string toString();
    template<class Archive>
    void serialize(Archive & archive){
        archive(this->type, this->size, this->senderID, this->categories);
    }
};


#endif //RING_CATEGORYLISTMESSAGE_H
