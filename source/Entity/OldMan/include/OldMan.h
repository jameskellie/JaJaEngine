#ifndef OLDMAN_H
#define OLDMAN_H

#include "Animation.h"
#include "Entity.h"
#include "RigidBody.h"

class OldMan : public Entity
{
    // Min = X/Y | Max = W/H
    const Vector2D hitboxMin = Vector2D({3.0f, 10.0f});
    const Vector2D hitboxMax = Vector2D({10.0f, 6.0f});

    std::unique_ptr<Animation> animation;
    std::unique_ptr<RigidBody> rigidBody;

    Vector2D lastPos;

    bool movingHorizontally = false;

public:
    OldMan(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties);

    // Observer-related functions
    void CollisionReaction(std::shared_ptr<Level> level) override;

    virtual void Update(std::shared_ptr<Resources> resources) override;
    virtual void Render(std::shared_ptr<Resources> resources) override;
    virtual void UpdateHitbox()                               override;
};

#endif // OLDMAN_H