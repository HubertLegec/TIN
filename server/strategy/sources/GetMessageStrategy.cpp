#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../headers/GetMessageStrategy.h"
#include "../../../networkMessage/headers/GetMessage.h"
#include "../../controller/headers/Controller.h"

using namespace std;

void GetMessageStrategy::serveEvent(SimpleMessage *message) const {
    GetMessage *getMessage = dynamic_cast<GetMessage *> (message);

    if (getMessage->getRequestType() == CAT_LIST) {
        map<long, std::string> categories;
        CategoryListMessage *returnMessage = new CategoryListMessage();
        returnMessage->setType(SERVER_INFO);
        returnMessage->setSenderID(getMessage->getSenderID());

        for (auto pair: controller->getModel()->getCategories()) {
            categories[pair.first] = pair.second->getName();
        }

        controller->sendMessage(returnMessage);
    } else {
        long categoryID; /* = getMessage->get */
        // TODO problem polega na tym, ze ID jest teraz globalne
        // Wiec, potrzeba przekazac wiadomosc, ktorej dokladnie kategorii
        // dotyczy pobranie sasiadow
        long senderID = getMessage->getSenderID();
/*
        auto member = controller->getModel()->getCategory(categoryID)->findMember(senderID);
        auto leftNeighbour = member->getLeftNeighbour()->getUser();
        auto rightNeighbout = member->getRightNeighbour()->getUser();

        NeighboursInfoMessage *infoMessage = new NeighboursInfoMessage(categoryID, leftNeighbour->getName(),
                                                                       leftNeighbour->getIP(), leftNeighbour->getPort(),
                                                                       rightNeighbout->getName(),
                                                                       rightNeighbout->getIP(),
                                                                       rightNeighbout->getPort());

        controller->putOutgoingMessage(infoMessage);
*/
//        throw runtime_error("Not supported yet!");
    }
}
