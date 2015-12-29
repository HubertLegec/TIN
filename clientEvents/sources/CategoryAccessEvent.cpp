#include "../headers/CategoryAccessEvent.h"
#include <sstream>

using namespace std;

CategoryAccessEvent::CategoryAccessEvent(CategoryAccessEvent::AccessType type_, const string &categoryName_)
        : BasicEvent(), type(type_), categoryName(categoryName_) { }

CategoryAccessEvent::CategoryAccessEvent(AccessType type_, long categoryID_)
        : BasicEvent(), type(type_), categoryId(categoryID_) { }

CategoryAccessEvent::AccessType CategoryAccessEvent::getType() {
    return type;
}

const std::string &CategoryAccessEvent::getCategoryName() const {
    return categoryName;
}

long CategoryAccessEvent::getCategoryID() const {
    return categoryId;
}

std::string CategoryAccessEvent::toString() const {
    stringstream ss;
    ss << "CategoryAccessEvent[type:" << type << "; ";
    ss << "categoryName:" << categoryName << "]";

}

std::string CategoryAccessEvent::getName() const {
    return "CATEGORY_ACCESS_EVENT";
}

