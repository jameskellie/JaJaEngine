#ifndef CAR_H
#define CAR_H

#include "Entity.h"

class Car : public Entity
{
    // Min = X/Y | Max = W/H
    const Vector2D hitboxMin = Vector2D({4.0f, 11.0f});
    const Vector2D hitboxMax = Vector2D({24.0f, 20.0f});

public:
    Car(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties);

    ~Car();

    // Observer-related functions
    void CollisionReaction(std::shared_ptr<Level> level) override;

    virtual void Update(std::shared_ptr<Resources> resources) override;
    virtual void Render(std::shared_ptr<Resources> resources) override;
    virtual void UpdateHitbox()                               override;
};

#endif // CAR_H