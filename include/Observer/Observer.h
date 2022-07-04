#ifndef OBSERVER_H
#define OBSERVER_H

#include "SDL_rect.h"

#include <memory>

struct Object;

class Observer
{
public:
    virtual ~Observer() {}
    virtual void Update(std::shared_ptr<void> collision) = 0;
};

#endif // OBSERVER_H