#include "../../../networkMessage/headers/CategoryListMessage.h"
#include "../headers/GetMessageStrategy.h"
#include "../../../networkMessage/headers/GetMessage.h"
#include "../../controller/headers/Controller.h"

using namespace std;

void GetMessageStrategy::serveEvent(SimpleMessage *message) const {
    GetMessage *getMessage = dynamic_cast<GetMessage *> (message);

    // TODO del..
    cout << "TEST OUTPUT" << endl;

    if (getMessage->getRequestType() == CAT_LIST) {
        map<long, std::string> categories;

        for (auto pair: controller->getModel()->getCategories()) {
            categories[pair.first] = pair.second->getName();
        }

        controller->putOutgoingMessage(new CategoryListMessage(-1, categories));
    } else {
        // TODO problem polega na tym, ze ID jest teraz globalne
        // Wiec, potrzeba przekazac wiadomosc, ktorej dokladnie kategorii
        // dotyczy pobranie sasiadow

//            long senderID = getMessage->getSenderID();
//            getMessage->get

//        throw runtime_error("Not supported yet!");
    }
}
