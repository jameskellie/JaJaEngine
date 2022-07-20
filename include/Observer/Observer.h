#ifndef OBSERVER_H
#define OBSERVER_H

#include "SDL_rect.h"

#include <memory>

class Level;

class Observer
{
public:
    virtual ~Observer() {}
    virtual void Update(std::shared_ptr<void> collision, std::shared_ptr<Level> level) = 0;
};

#endif // OBSERVER_H