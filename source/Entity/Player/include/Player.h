#ifndef PLAYER_H
#define PLAYER_H

#include "Animation.h"
#include "Entity.h"
#include "RigidBody.h"

class Player : public Entity
{
    const float runSpeed = 64.0f;

    // TODO: This could be moved to entity when/if needed for other entities
    // Min = X/Y | Max = W/H
    const Vector2D hitboxMin = Vector2D({3.0f, 10.0f});
    const Vector2D hitboxMax = Vector2D({14.0f, 2.0f});

    std::unique_ptr<Animation> animation;
    std::unique_ptr<RigidBody> rigidBody;

    Vector2D lastPos;

    bool movingHorizontally,
         walkingUp,
         walkingDown,
         walkingLeft,
         walkingRight;

public:
    Player(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties);

    // Observer-related functions
    void CollisionReaction() override;

    void Idle();
    void Movement(std::shared_ptr<Resources> resources);

    virtual void Update(std::shared_ptr<Resources> resources, std::shared_ptr<Quadtree> quadtree) override;
    virtual void Render(std::shared_ptr<Resources> resources)                                     override;

    virtual inline SDL_FRect GenerateHitbox() override { return SDL_FRect({(transform->x + hitboxMin.x), (transform->y + hitboxMax.x), hitboxMin.y, hitboxMax.y}); }
};

#endif // PLAYER_H