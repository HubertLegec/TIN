#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../headers/GetMessageStrategy.h"
#include "../../../networkMessage/headers/GetMessage.h"

void GetMessageStrategy::serveEvent(SimpleMessage *message) {
    GetMessage *getMessage = dynamic_cast<GetMessage *> (message);

    if (getMessage->getRequestType() == CAT_LIST) {
        map<long, std::string> categories;

        for (auto pair: controller->getModel()->getCategories()) {
            categories[pair.first] = pair.second->getName();
        }

        CategoryListMessage listMessage(-1, categories);
        controller->putOutgoingMessage(listMessage);
    } else {
        // TODO problem polega na tym, ze ID jest teraz globalne
        // Wiec, potrzeba przekazac wiadomosc, ktorej dokladnie kategorii
        // dotyczy pobranie sasiadow

//            long senderID = getMessage->getSenderID();
//            getMessage->get

        throw runtime_error("Not supported yet!");
    }
}
