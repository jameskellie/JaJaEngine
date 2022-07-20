#ifndef OBJECT_H
#define OBJECT_H

#include "Entity.h"

#include <string>

class Object : public Entity
{
public:
    Object() : Entity() {}

    Object(const SDL_FRect &hitbox)
        : Entity(hitbox) {}

    // Observer-related functions
    void CollisionReaction(std::shared_ptr<Level> level) override;

    virtual void Update(std::shared_ptr<Resources> resources) override;
    virtual void Render(std::shared_ptr<Resources> resources) override;
    virtual void UpdateHitbox()                               override;
};

#endif // OBJECT_H