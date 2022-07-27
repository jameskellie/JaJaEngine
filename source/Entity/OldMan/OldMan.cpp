#include "OldMan.h"

#include "Resources.h"

OldMan::OldMan(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)
    : Entity(subject, properties)
{
    facing    = Direction::SOUTH;
    animation = std::make_unique<Animation>(properties.id, states);
    rigidBody = std::make_unique<RigidBody>();
    animation->SetState("idle_south");
}

void OldMan::CollisionReaction(std::shared_ptr<Level> level)
{
    (void)level;

    if (collision->loadZone == "")
    {
        if (movingHorizontally) transform->x = (lastPos.x + (hitboxMin.x + hitboxMax.x) - 1.0f <= collision->hitbox.x)
                                             ? collision->hitbox.x - (hitboxMin.x + hitboxMax.x)
                                             : (collision->hitbox.x + collision->hitbox.w) - hitboxMin.x;
        else                    transform->y = (lastPos.y + (hitboxMin.y + hitboxMax.y) - 1.0f <= collision->hitbox.y)
                                             ? collision->hitbox.y - (hitboxMin.y + hitboxMax.y)
                                             : (collision->hitbox.y + collision->hitbox.h) - hitboxMin.y;
    }
}

void OldMan::Update(std::shared_ptr<Resources> resources)
{
    movingHorizontally = !movingHorizontally;

    rigidBody->Update(resources->GetEngine()->GetDeltaTime());

    // X Collision
    if (movingHorizontally)
    {
        lastPos.x = transform->x;
        transform->TranslateX(rigidBody->GetPosition().x);
    }
    // Y Collision
    else
    {
        lastPos.y = transform->y;
        transform->TranslateY(rigidBody->GetPosition().y);        
    }

    animation->Update();
    UpdateHitbox();
}

void OldMan::Render(std::shared_ptr<Resources> resources)
{
    animation->Draw(resources, transform->x, transform->y, tileWidth, tileHeight);
}

void OldMan::UpdateHitbox()
{
    hitbox.x = transform->x + hitboxMin.x;
    hitbox.y = transform->y + hitboxMin.y;
    hitbox.w = hitboxMax.x;
    hitbox.h = hitboxMax.y;
}