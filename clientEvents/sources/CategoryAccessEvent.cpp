#include "../headers/CategoryAccessEvent.h"
#include <iostream>

CategoryAccessEvent::CategoryAccessEvent(CategoryAccessEvent::AccessType type_, const std::string & categoryName_, const std::string & userName_,const std::string & userPassword_)
:BasicEvent(), type(type_), categoryName(categoryName_), userName(userName_), userPassword(userPassword_) { }

CategoryAccessEvent::AccessType CategoryAccessEvent::getType() { return type;}
std::string CategoryAccessEvent::getCategoryName() { return categoryName;}
std::string CategoryAccessEvent::getUserName() {return userName;};
std::string CategoryAccessEvent::getPassword() {return userPassword;};

std::string CategoryAccessEvent::toString() {return "CategoryAccessEvent"; }

