#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"

class Subject
{
public:
    virtual ~Subject() {}
    virtual void Attach(Observer *observer) = 0;
    virtual void Detach(Observer *observer) = 0;
    virtual void Notify()                   = 0;
};

#endif // SUBJECT_H