#include "OldMan.h"

#include "Resources.h"

OldMan::OldMan(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)
    : Entity(subject, properties)
{
    loadZone  = "OLDMAN";
    facing    = Direction::SOUTH;
    animation = std::make_unique<Animation>(properties.id, states);
    rigidBody = std::make_unique<RigidBody>();
    animation->SetState("idle_south");
}

OldMan::~OldMan()
{
    RemoveObserver();
}

void OldMan::CollisionReaction(std::shared_ptr<Level> level)
{
    (void)level;

    if (collision->loadZone == "")
    {
        // Colliding with left edge
        if (lastPos.x + hitboxMin.x + hitboxMax.x <= collision->hitbox.x && lastPos.y + hitboxMin.y + hitboxMax.y > collision->hitbox.y && lastPos.y + hitboxMin.y < collision->hitbox.y + collision->hitbox.h)
            transform->x = collision->hitbox.x - (hitboxMin.x + hitboxMax.x);
        // Colliding with right edge
        if (lastPos.x + hitboxMin.x >= collision->hitbox.x + collision->hitbox.w && lastPos.y + hitboxMin.y + hitboxMax.y > collision->hitbox.y && lastPos.y + hitboxMin.y < collision->hitbox.y + collision->hitbox.h)
            transform->x = (collision->hitbox.x + collision->hitbox.w) - hitboxMin.x;
        // Colliding with top edge
        if (lastPos.y + hitboxMin.y + hitboxMax.y <= collision->hitbox.y && lastPos.x + hitboxMin.x + hitboxMax.x > collision->hitbox.x && lastPos.x + hitboxMin.x < collision->hitbox.x + collision->hitbox.w)
            transform->y = collision->hitbox.y - (hitboxMin.y + hitboxMax.y);
        // Colliding with bottom edge
        if (lastPos.y + hitboxMin.y >= collision->hitbox.y + collision->hitbox.h && lastPos.x + hitboxMin.x + hitboxMax.x > collision->hitbox.x && lastPos.x + hitboxMin.x < collision->hitbox.x + collision->hitbox.w)
            transform->y = (collision->hitbox.y + collision->hitbox.h) - hitboxMin.y;
    }
}

void OldMan::Update(std::shared_ptr<Resources> resources)
{
    rigidBody->Update(resources->GetEngine()->GetDeltaTime());
    rigidBody->ReduceForceX(64.0f * resources->GetEngine()->GetDeltaTime());
    rigidBody->ReduceForceY(64.0f * resources->GetEngine()->GetDeltaTime());

    lastPos.x  = transform->x;
    lastPos.y  = transform->y;
    lastHitbox = hitbox;

    transform->Translate(rigidBody->GetPosition());

    SetOrigin();
    animation->Update();
    UpdateHitbox();
}

void OldMan::Render(std::shared_ptr<Resources> resources)
{
    animation->Draw(resources, lastPos.x, lastPos.y, tileWidth, tileHeight);
}

void OldMan::UpdateHitbox()
{
    hitbox.x = transform->x + hitboxMin.x;
    hitbox.y = transform->y + hitboxMin.y;
    hitbox.w = hitboxMax.x;
    hitbox.h = hitboxMax.y;
}