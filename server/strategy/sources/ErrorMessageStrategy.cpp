#include "../headers/ErrorMessageStrategy.h"
#include "../../../logger/easylogging++.h"
#include "../../../networkMessage/headers/NetworkControllerErrorMessage.h"
#include "../../controller/headers/Controller.h"

void ErrorMessageStrategy::serveEvent(SimpleMessage *message) const {
    NetworkControllerErrorMessage *errorMessage = dynamic_cast<NetworkControllerErrorMessage *>(message);
    controller->cleanUp();
    LOG(FATAL) << "Received error message from network controler. Message info: " << errorMessage->getInfo();
}
