#include "../headers/CategoryListMessageStrategy.h"
#include "../../../networkMessage/headers/CategoryListMessage.h"

void CategoryListMessageStrategy::serveEvent(SimpleMessage *message) {
    CategoryListMessage *categoryListMessage = dynamic_cast<CategoryListMessage *> (message);


}
