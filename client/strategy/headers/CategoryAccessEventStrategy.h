//
// Created by hubert on 29.12.15.
//

#ifndef RING_CATEGORYACCESSEVENTSTRATEGY_H
#define RING_CATEGORYACCESSEVENTSTRATEGY_H

#include "BasicEventStrategy.h"

class CategoryAccessEventStrategy : public BasicEventStrategy {
private:
    void createCategory(std::string name) const;

    void deleteCategory(long id) const;

    void signUpCategory(long id) const;

    void signOutCategory(long id) const;

    void joinCategory(long id) const;

    void leaveCategory(long id) const;

public:
    CategoryAccessEventStrategy();

    CategoryAccessEventStrategy(Controller *controller);

    virtual void serveEvent(BasicEvent *event) override;
};


#endif //RING_CATEGORYACCESSEVENTSTRATEGY_H
