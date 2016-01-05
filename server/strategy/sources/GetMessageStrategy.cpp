#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../headers/GetMessageStrategy.h"
#include "../../../networkMessage/headers/GetMessage.h"
#include "../../controller/headers/Controller.h"
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

            controller->sendMessage(returnMessage, senderID);
        }
            break;

        case GetMessageType::NEIGHBOURS: {
            long categoryID = getMessage->getCategoryID();

            try {
                sendNeighbours(categoryID, senderID);
                LOG(INFO) << "Sent neighbours set to user " << senderID;
            } catch (out_of_range &e) {
                LOG(INFO) << "Couldn't get member's neighbours " << senderID << " in cateogry " << categoryID;
                sendMessage(senderID, FAIL, "Couldn't get neighbours");
            }
        }
            break;

        default: {
            sendMessage(senderID, FAIL, "Bad message type received");
        }
            break;
    }
}
