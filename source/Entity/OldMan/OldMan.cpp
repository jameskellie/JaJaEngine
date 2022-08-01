#include "OldMan.h"

#include "Resources.h"
#include <iostream> // TODO: DELETE
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

    if (collision->loadZone == "PLAYER")
    {
        if (movingHorizontally)
        {
            collisionHorizontally = true;

            if (lastPos.x + hitboxMin.x <= collision->hitbox.x)
            {
                rigidBody->ApplyForceX(-32.0f);
            }
            else
            {
                rigidBody->ApplyForceX(32.0f);
            }
        }
        else
        {
            collisionHorizontally = false;

            if (lastPos.y + hitboxMin.y <= collision->hitbox.y)
            {
                rigidBody->ApplyForceY(-32.0f);
            }
            else
            {
                rigidBody->ApplyForceY(32.0f);
            }
        }
    }
    else
    {        
        if (collisionHorizontally) transform->x = (lastPos.x + hitboxMin.x <= collision->hitbox.x)
                                                ? collision->hitbox.x - (hitboxMin.x + hitboxMax.x)
                                                : (collision->hitbox.x + collision->hitbox.w) - hitboxMin.x;
        else                       transform->y = (lastPos.y + hitboxMin.y <= collision->hitbox.y)
                                                ? collision->hitbox.y - (hitboxMin.y + hitboxMax.y)
                                                : (collision->hitbox.y + collision->hitbox.h) - hitboxMin.y;
    }
}

void OldMan::Update(std::shared_ptr<Resources> resources)
{
    movingHorizontally = !movingHorizontally;

    rigidBody->Update(resources->GetEngine()->GetDeltaTime());
    rigidBody->RemoveForce();

    lastPos.x = transform->x;
    lastPos.y = transform->y;

    transform->Translate(rigidBody->GetPosition());

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