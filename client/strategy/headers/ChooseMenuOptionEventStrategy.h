//
// Created by hubert.legec on 2015-12-15.
//

#ifndef RING_CHOOSEMENUOPTIONEVENTSTRATEGY_H
#define RING_CHOOSEMENUOPTIONEVENTSTRATEGY_H

#include "BasicEventStrategy.h"


class ChooseMenuOptionEventStrategy : public BasicEventStrategy {
private:
    void showCategoryList() const;
    void createCategory() const;
    void deleteCategory() const;
    void joinCategory() const;
    void leaveCategory() const;

    void signUpCategory() const;

    void signOutCategory() const;

    void createAccount() const;

    void sendRingMessage() const;

    void refresh() const;

public:
    ChooseMenuOptionEventStrategy();
    ChooseMenuOptionEventStrategy(Controller* controller);

    virtual void serveEvent(BasicEvent *event) override;
};


#endif //RING_CHOOSEMENUOPTIONEVENTSTRATEGY_H
