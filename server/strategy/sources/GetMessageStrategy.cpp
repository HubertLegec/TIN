#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../headers/GetMessageStrategy.h"
#include "../../../networkMessage/headers/GetMessage.h"
#include "../../controller/headers/Controller.h"
#include "../../../logger/easylogging++.h"
#include "../../utils/ServerGlobalConstants.h"

using namespace std;

void GetMessageStrategy::serveEvent(SimpleMessage *message) const {
    GetMessage *getMessage = dynamic_cast<GetMessage *> (message);

    if (checkSender(message) == ServerGlobalConstant::FAILED_CODE)
        return;

    auto requestType = getMessage->getRequestType();
    switch (requestType) {
        case GetMessageType::CAT_LIST: {
            sendCategoriesList();
        }
            break;

        case GetMessageType::NEIGHBOURS: {
            long categoryID = getMessage->getCategoryID();
            sendNeighbours(categoryID);
        }
            break;

        default: {
            badMessageTypeReceived();
        }
            break;
    }
}

void GetMessageStrategy::sendCategoriesList() const {
    CategoryListMessage *returnMessage = new CategoryListMessage();
    returnMessage->setType(MessageType::CATEGORY_LIST);
    returnMessage->setSenderID(ServerGlobalConstant::SERVER_ID);

    for (auto pair: controller->getModel()->getCategories()) {
        returnMessage->addCategory(pair.first, pair.second->getName());
    }

    controller->sendMessage(shared_ptr<CategoryListMessage>(returnMessage), getSender());
    LOG(INFO) << "Sent categories list to user " << getSender()->getID();
}

void GetMessageStrategy::sendNeighbours(long categoryID) const {
    auto category = controller->getModel()->getCategory(categoryID);
    if (!category) {
        sendCategoryNotFound(getSender(), categoryID);
        return;
    }
    auto member = category->findMember(getSender()->getID());
    if (!member) {
        sendMemberNotFound(getSender(), categoryID, getSender()->getID());
        return;
    }

    BasicEventStrategy::sendNeighbours(category, member);
    LOG(INFO) << "Sent neighbours set to user " << getSender()->getID();
}
