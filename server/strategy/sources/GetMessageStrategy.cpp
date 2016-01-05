#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../headers/GetMessageStrategy.h"
#include "../../../networkMessage/headers/GetMessage.h"
#include "../../controller/headers/Controller.h"
#include "../../../networkMessage/headers/NeighboursInfoMessage.h"
#include "../../../logger/easylogging++.h"

using namespace std;

void GetMessageStrategy::serveEvent(SimpleMessage *message) const {
    GetMessage *getMessage = dynamic_cast<GetMessage *> (message);

    auto senderID = getMessage->getSenderID();
    auto requestType = getMessage->getRequestType();

    switch (requestType) {
        case GetMessageType::CAT_LIST: {
            CategoryListMessage *returnMessage = new CategoryListMessage();
            returnMessage->setType(CATEGORY_LIST);
            returnMessage->setSenderID(SERVER_ID);

            for (auto pair: controller->getModel()->getCategories()) {
                returnMessage->addCategory(pair.first, pair.second->getName());
            }

            LOG(INFO) << "Sent category list to user " << senderID;
            controller->sendMessage(returnMessage, senderID);
        }
            break;

        case GetMessageType::NEIGHBOURS: {
            long categoryID = getMessage->getCategoryID();

            sendNeighbours(categoryID, senderID);
            LOG(INFO) << "Sent neighbours set to user " << senderID;
        }
            break;

        default: {
            sendMessage(senderID, FAIL, "Bad message type received");
        }
            break;
    }
}
