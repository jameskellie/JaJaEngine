#include "Car.h"

#include "Resources.h"

Car::Car(std::shared_ptr<Subject> subject, const std::unordered_map<std::string, Sequence> &states, const TextureProperties &properties)
    : Entity(subject, properties)
{
    loadZone  = "CAR";
    facing    = Direction::WEST;
    animation = std::make_unique<Animation>(properties.id, states);
    rigidBody = std::make_unique<RigidBody>();
    animation->SetState("drive_west");
    rigidBody->ApplyForceX(-128.0f);
}

Car::~Car()
{
    RemoveObserver();
}

void Car::CollisionReaction(std::shared_ptr<Level> level)
{
    (void)level;

    if (collision->loadZone == "PLAYER")
    {
        // Vector2D collisionForce = collision->rigidBody->GetForce();
        // collision->rigidBody->ApplyForceX(-130.0f + abs(collisionForce.x));
    }
}

void Car::Update(std::shared_ptr<Resources> resources)
{
    rigidBody->Update(resources->GetEngine()->GetDeltaTime());

    lastPos.x = transform->x;
    lastPos.y = transform->y;

    transform->Translate(rigidBody->GetPosition());

    if (transform->x < -50.0f)
    {
        transform->x = 550.0f;
    }

    animation->Update();
    UpdateHitbox();
}

void Car::Render(std::shared_ptr<Resources> resources)
{
    animation->Draw(resources, transform->x, transform->y, tileWidth, tileHeight);
}

void Car::UpdateHitbox()
{
    switch (facing)
    {
        case Direction::EAST:
        case Direction::WEST:
        hitboxMin = Vector2D({4.0f, 16.0f});
        hitboxMax = Vector2D({24.0f, 15.0f});
        break;

        case Direction::NORTH:
        case Direction::SOUTH:
        hitboxMin = Vector2D({9.0f, 11.0f});
        hitboxMax = Vector2D({14.0f, 18.0f});
        break;
    }
    
    hitbox.x = transform->x + hitboxMin.x;
    hitbox.y = transform->y + hitboxMin.y;
    hitbox.w = hitboxMax.x;
    hitbox.h = hitboxMax.y;
}