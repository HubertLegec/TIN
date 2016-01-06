#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../headers/GetMessageStrategy.h"
#include "../../../networkMessage/headers/GetMessage.h"
#include "../../controller/headers/Controller.h"
#include "../../../logger/easylogging++.h"

using namespace std;

void GetMessageStrategy::serveEvent(SimpleMessage *message) const {
    GetMessage *getMessage = dynamic_cast<GetMessage *> (message);

    auto senderID = getMessage->getSenderID();
    auto sender = controller->getModel()->getUser(senderID);
    if (!sender) {
        LOG(INFO) << "Couldn't find user who sent message. SenderID: " << senderID;
        return;
    }

    auto requestType = getMessage->getRequestType();
    switch (requestType) {
        case GetMessageType::CAT_LIST: {
            CategoryListMessage *returnMessage = new CategoryListMessage();
            returnMessage->setType(CATEGORY_LIST);
            returnMessage->setSenderID(SERVER_ID);

            for (auto pair: controller->getModel()->getCategories()) {
                returnMessage->addCategory(pair.first, pair.second->getName());
            }

            controller->sendMessage(returnMessage, senderID);
        }
            break;

        case GetMessageType::NEIGHBOURS: {
            long categoryID = getMessage->getCategoryID();
            auto category = controller->getModel()->getCategory(categoryID);
            if (!category) {
                sendCategoryNotFound(sender, categoryID);
                return;
            }
            auto member = category->findMember(senderID);
            if(!member) {
                sendMemberNotFound(sender, categoryID, senderID);
                return;
            }

            sendNeighbours(category, member);
            LOG(INFO) << "Sent neighbours set to user " << senderID;
        }
            break;

        default: {
            sendMessage(sender, FAIL, "Bad message type received");
        }
            break;
    }
}
