//
// Created by hubert on 05.12.15.
//

#ifndef RING_BASICEVENT_H
#define RING_BASICEVENT_H

#include <string>

/**
 * Base class of every event in client
 * It's an abstract class
 */
class BasicEvent {
public:
    /**
     * Derived classes should implement this method.
     * It should return class name, and list of class fields names and values
     */
    virtual std::string toString() const = 0;

    /**
     * Derived classes should implement this method.
     * It should return uppercase name of event class
     */
    virtual std::string getName() const = 0;
};


#endif //RING_BASICEVENT_H
