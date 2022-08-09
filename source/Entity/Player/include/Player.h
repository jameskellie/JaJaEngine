#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity
{
    const float runSpeed = 64.0f;

    // Min = X/Y | Max = W/H
    const Vector2D hitboxMin = Vector2D({3.0f, 10.0f});
    const Vector2D hitboxMax = Vector2D({10.0f, 6.0f});

public:
    Player(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties);

    ~Player();

    // Observer-related functions
    void CollisionReaction(std::shared_ptr<Level> level) override;

    void Idle();
    void Movement(std::shared_ptr<Resources> resources);

    virtual void Update(std::shared_ptr<Resources> resources) override;
    virtual void Render(std::shared_ptr<Resources> resources) override;
    virtual void UpdateHitbox()                               override;
};

#endif // PLAYER_H