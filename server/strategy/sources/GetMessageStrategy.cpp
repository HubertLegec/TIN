#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../headers/GetMessageStrategy.h"
#include "../../../networkMessage/headers/GetMessage.h"
#include "../../controller/headers/Controller.h"
#include "../../../networkMessage/headers/NeighboursInfoMessage.h"
#include "../../../logger/easylogging++.h"

using namespace std;

void GetMessageStrategy::serveEvent(SimpleMessage *message) const {
    GetMessage *getMessage = dynamic_cast<GetMessage *> (message);
    auto userID = getMessage->getSenderID();
    auto requestType = getMessage->getRequestType();

    if (requestType == CAT_LIST) {
//        map<long, string> categories;
        CategoryListMessage *returnMessage = new CategoryListMessage();
        returnMessage->setType(CATEGORY_LIST);
        returnMessage->setSenderID(SERVER_ID);

        for (auto pair: controller->getModel()->getCategories()) {
//            categories[pair.first] = pair.second->getName();
            returnMessage->addCategory(pair.first,pair.second->getName());
        }

//        returnMessage->set
        LOG(INFO) << "Sent category list to user " << userID;
        controller->sendMessage(returnMessage, userID);
    } else if (requestType == NEIGHBOURS) {
        long categoryID = getMessage->getCategoryID();
        long senderID = getMessage->getSenderID();

        auto member = controller->getModel()->getCategory(categoryID)->findMember(senderID);
        auto leftNeighbour = member->getLeftNeighbour()->getUser();
        auto rightNeighbour = member->getRightNeighbour()->getUser();

        NeighboursInfoMessage *infoMessage = new NeighboursInfoMessage(categoryID,
                                                                       leftNeighbour->getName(),
                                                                       leftNeighbour->getIP(),
                                                                       leftNeighbour->getPort(),
                                                                       rightNeighbour->getName(),
                                                                       rightNeighbour->getIP(),
                                                                       rightNeighbour->getPort());
        infoMessage->setSenderID(SERVER_ID);
        infoMessage->setType(NEIGHBOURS_SET);

        LOG(INFO) << "Sent neighbours set to user " << userID;
        controller->sendMessage(infoMessage, userID);
    } else {
        ServerInfoMessage *returnMessage = new ServerInfoMessage(SERVER_ID, FAIL, "Bad message type received");
        returnMessage->setType(SERVER_INFO);
        LOG(DEBUG) << "Received bad message type from user " << userID;
        controller->sendMessage(returnMessage);
    }
}
