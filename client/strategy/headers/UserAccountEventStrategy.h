//
// Created by hubert on 29.12.15.
//

#ifndef RING_USERACCOUNTEVENTSTRATEGY_H
#define RING_USERACCOUNTEVENTSTRATEGY_H

#include "BasicEventStrategy.h"
#include "../../../clientEvents/headers/BasicEvent.h"

class UserAccountEventStrategy : public BasicEventStrategy {
private:
    void createAccount(const std::string &name) const;

    void deleteAccount() const;

public:
    UserAccountEventStrategy();

    UserAccountEventStrategy(Controller *controller);

    virtual void serveEvent(BasicEvent *event) override;
};


#endif //RING_USERACCOUNTEVENTSTRATEGY_H
