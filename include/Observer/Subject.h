#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"

class Entity;
class Level;

class Subject
{
public:
    virtual ~Subject() {}
    virtual void Attach(Entity *observer)             = 0;
    virtual void Detach(Entity *observer)             = 0;
    virtual void Notify(std::shared_ptr<Level> level) = 0;
};

#endif // SUBJECT_H