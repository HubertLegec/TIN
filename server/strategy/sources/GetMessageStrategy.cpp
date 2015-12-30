#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../headers/GetMessageStrategy.h"
#include "../../../networkMessage/headers/GetMessage.h"
#include "../../controller/headers/Controller.h"
#include "../../../networkMessage/headers/NeighboursInfoMessage.h"

using namespace std;

void GetMessageStrategy::serveEvent(SimpleMessage *message) const {
    GetMessage *getMessage = dynamic_cast<GetMessage *> (message);
    auto userID = getMessage->getSenderID();

    if (getMessage->getRequestType() == CAT_LIST) {
        map<long, string> categories;
        CategoryListMessage *returnMessage = new CategoryListMessage();
        returnMessage->setType(CATEGORY_LIST);
        returnMessage->setSenderID(SERVER_ID);

        for (auto pair: controller->getModel()->getCategories()) {
            categories[pair.first] = pair.second->getName();
        }

        controller->sendMessage(returnMessage, userID);
    } else {
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
        infoMessage->setSenderID(SERVER_INFO);
        infoMessage->setType(NEIGHBOURS_SET);

        controller->sendMessage(infoMessage, userID);
    }
}
