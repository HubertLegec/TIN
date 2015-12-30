#ifndef RING_SERVERUNKNOWNEXCEPTION_H
#define RING_SERVERUNKNOWNEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class ServerUnknownException : public exception {
public:
    virtual string what();
};


#endif //RING_SERVERUNKNOWNEXCEPTION_H
